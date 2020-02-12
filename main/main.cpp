#include "main.h"
#include "animate.h"
#include "log.h"
#include "global-events.h"
#include "game-def.h"
#include "state-common.h"
#include "state-enumerate.h"
#include "state-fail.h"
#include "state-player-assign.h"
#include "state-mover.h"
#include "state-board.h"
#include "state-progress.h"

namespace _main_ {
    
    void setup() {
        stateCommon::addState(stateFail::loop, nullptr, GAME_DEF_STATE_FAIL);
        stateCommon::addState(stateEnumerate::loop, stateEnumerate::enter, GAME_DEF_STATE_ENUM_NONE);
        stateCommon::addState(statePlayerAssign::loop, statePlayerAssign::enter, GAME_DEF_STATE_PLAYER_ASSIGN);
        stateCommon::addState(stateMover::loop, stateMover::enter, GAME_DEF_STATE_MOVER);
        stateCommon::addState(stateBoard::loop, stateBoard::enter, GAME_DEF_STATE_BOARD);
        stateCommon::addState(stateProgress::loop, stateProgress::enter, GAME_DEF_STATE_PROGRESS);
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    
    void loop() {
        FOREACH_FACE(face) {
            if(!isDatagramReadyOnFace(face)) {
                continue;
            }
            if(getDatagramLengthOnFace(face) != ACTION_LEN) {
                markDatagramReadOnFace(face);
                continue;
            }
            const byte* buffer = getDatagramOnFace(face);
            const action::Action action = action::decode(buffer);
            markDatagramReadOnFace(face);
            stateCommon::LoopData data{.face=face, .action=action};
            if(!globalEvents::isHandled(data)) {
                stateCommon::loop(data);
            }
        }
        stateCommon::LoopData empty{.face=FACE_COUNT, .action=action::Action{.type=GAME_DEF_ACTION_INVALID}};
        if(!globalEvents::isHandled(empty)) {
            stateCommon::loop(empty);
        }
        
    }

}
