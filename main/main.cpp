#include "main.h"
#include "animate.h"
#include "log.h"
#include "global-events.h"
#include "game-def.h"
#include "state-common.h"
#include "state-player-assign.h"
#include "state-mover.h"
#include "state-board.h"
#include "state-progress.h"
#include "state-end.h"
#include "timer.h"

namespace _main_ {
    
    void setup() {
        stateCommon::addState(statePlayerAssign::loop, GAME_DEF_STATE_PLAYER_ASSIGN);
        stateCommon::addState(stateMover::loop, GAME_DEF_STATE_MOVER);
        stateCommon::addState(stateBoard::loop, GAME_DEF_STATE_BOARD);
        stateCommon::addState(stateProgress::loop, GAME_DEF_STATE_PROGRESS);
        stateCommon::addState(stateEnd::loop, GAME_DEF_STATE_END);
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    
    void loop() {
        timer::loop();
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
