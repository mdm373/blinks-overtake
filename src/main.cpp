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
namespace _main_ {
    
    void setup() {
        stateCommon::addState(stateFail::loop, nullptr, nullptr, GAME_DEF_STATE_FAIL);
        stateCommon::addState(stateEnumerate::loopNone, stateEnumerate::enterNone, nullptr, GAME_DEF_STATE_ENUM_NONE);
        stateCommon::addState(stateEnumerate::loopPending, nullptr, nullptr, GAME_DEF_STATE_ENUM_PEND);
        stateCommon::addState(stateEnumerate::loopAssigned, nullptr, nullptr, GAME_DEF_STATE_ENUM_ASGN);
        stateCommon::addState(statePlayerAssign::loop, statePlayerAssign::enter, nullptr, GAME_DEF_STATE_PLAYER_ASSIGN);
        stateCommon::addState(stateMover::loop, stateMover::enter, nullptr, GAME_DEF_STATE_MOVER);
        stateCommon::addState(stateBoard::loop, stateBoard::enter, nullptr, GAME_DEF_STATE_BOARD);
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
