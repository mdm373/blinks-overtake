#include "state-common.h"
#include "log.h"
#include "animate.h"

namespace stateCommon {

    stateLoop loops[STATE_COMMON_STATE_LEN];
    stateAction enters[STATE_COMMON_STATE_LEN];
    stateAction exits[STATE_COMMON_STATE_LEN];
    
    byte currentState = 0;
    
    void handleStateChange(const byte newState) {
        LOG_LINE("changed to state:", newState);
        if(exits[currentState] != nullptr) {
            exits[currentState]();
        }
        currentState = newState;
        if(enters[currentState] != nullptr) {
            enters[currentState]();
        }
    }

    byte getCurrent() {
        return currentState;
    }

    void addState(const stateLoop loop, const stateAction enter, const stateAction exit, const byte stateIndex) {
        loops[stateIndex] = loop;
        enters[stateIndex] = enter;
        exits[stateIndex] = exit;
    }

    void loop(const LoopData& data) {
        if(loops[currentState] == nullptr) {
            animate::pulse(ORANGE, 4);
            return;
        }
        loops[currentState](data);
    }
}