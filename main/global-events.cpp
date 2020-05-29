#include "global-events.h"
#include "game-def.h"
#include "state-board.h"
#include "animate.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace globalEvents {

    bool _pendingReset = false;

    void changeAllToReset(){
        action::broadcast( (byte) GAME_DEF_ACTION_RESET,  (byte) millis() );
        stateBoard::reset();
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    bool checkWake() {
        bool isWoken = hasWoken();
        bool isLongPressed = buttonLongPressed();
        bool isReleased = buttonReleased();
        if(isWoken){
            _pendingReset = false;
        }
        if(isLongPressed) {
            _pendingReset = true;
        }
        if(_pendingReset) {
            animate::pulse(WHITE, 2);
            if(isReleased){
                changeAllToReset();
                _pendingReset = false;
            }
            return true;
        }
        return false;
    }

    bool isHandled(const stateCommon::LoopData& data){
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_RESET)) {
            stateBoard::reset();
            stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
            return true;
        }
        return false;
    }

}