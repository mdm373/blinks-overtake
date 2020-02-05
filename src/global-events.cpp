#include "global-events.h"
#include "game-def.h"

#pragma clang diagnostic ignored "-Wc++11-narrowing"

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
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_FAIL)) {
            stateCommon::handleStateChange(GAME_DEF_STATE_FAIL);
            return true;
        }
        return false;
    }

    void changeAllToReset(){
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_RESET, .payload=millis()});
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    void changeAllToFail(){
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_FAIL, .payload=millis()});
        stateCommon::handleStateChange(GAME_DEF_STATE_FAIL);
    }
}