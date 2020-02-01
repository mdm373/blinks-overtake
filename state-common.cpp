#include "state-common.h"
#include <string.h>

namespace stateCommon {
    
    stateLoop loops[STATE_COMMON_INDEX_LENGTH];
    stateAction enters[STATE_COMMON_INDEX_LENGTH];
    stateAction exits[STATE_COMMON_INDEX_LENGTH];
    
    byte currentState = 0;
    
    void handleStateChange(const byte newState) {
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
        if(loops[currentState] != nullptr) {
            loops[currentState](data);
        }
    }
}