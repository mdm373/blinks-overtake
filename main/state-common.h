
#ifndef HEADER_STATE_COMMON
    #define HEADER_STATE_COMMON

    #include "blink-types.h"
    #include "action.h"

    namespace stateCommon {
        
        #define STATE_COMMON_STATE_LEN 5

        struct LoopData{
            const byte face;
            const action::Action action;
        };

        typedef void (*stateLoop)(bool isEnter, const LoopData& LoopData);
        
        void handleStateChange(const byte newState);
        void addState(const stateLoop loop, const byte stateIndex);
        void loop(const LoopData& loopData);
        
    }

#endif