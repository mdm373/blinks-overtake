#include "state-common.h"
#include "log.h"

namespace stateCommon {

    stateLoop _loops[STATE_COMMON_STATE_LEN];
    
    byte _currentState = 0;
    bool _isEnter = true;
    
    void handleStateChange(const byte newState) {
        LOG_LINE("changed to state:", newState);
        _currentState = newState;
        _isEnter = true;
    }

    void addState(const stateLoop loop, const byte stateIndex) {
        _loops[stateIndex] = loop;
    }

    void loop(const LoopData& data) {
        bool wasEnter = _isEnter;
        _isEnter = false;
        _loops[_currentState](wasEnter, data);
    }
}