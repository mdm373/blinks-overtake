#include "global-events.h"
#include "game-def.h"
#include "timer.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace globalEvents {

    bool isHandled(const stateCommon::LoopData& data){
        if(buttonDoubleClicked()) {
            changeAllToReset();
            return true;
        } 
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_RESET)) {
            stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
            return true;
        }
        return false;
    }

    void changeAllToReset(){
        timer::cancel();
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_RESET, .payload=millis()});
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }
}