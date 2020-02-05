#include "state-common.h"
#include "log.h"

namespace stateCommon {

    stateLoop _loops[STATE_COMMON_STATE_LEN];
    stateAction _enters[STATE_COMMON_STATE_LEN];
    
    byte _currentState = 0;
    
    void handleStateChange(const byte newState) {
        LOG_LINE("changed to state:", newState);
        _currentState = newState;
        if(_enters[_currentState] != nullptr) {
            _enters[_currentState]();
        }
    }

    byte getCurrent() {
        return _currentState;
    }

    void addState(const stateLoop loop, const stateAction enter, const byte stateIndex) {
        _loops[stateIndex] = loop;
        _enters[stateIndex] = enter;
    }

    void loop(const LoopData& data) {
        if(_loops[_currentState] != nullptr) {
            _loops[_currentState](data);
        }
    }
}