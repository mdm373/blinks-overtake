
#ifndef HEADER_STATE_COMMON
    #define HEADER_STATE_COMMON

    #include "blink-types.h"
    #include "action.h"

    namespace stateCommon {
        
        #define STATE_COMMON_STATE_LEN 7

        struct LoopData{
            const byte face;
            const action::Action action;
        };

        typedef void (*stateLoop)(const LoopData& LoopData);
        typedef void (*stateAction)(void);


        void handleStateChange(const byte newState);
        byte getCurrent();
        void addState(const stateLoop loop, const stateAction enter, const stateAction exit, const byte stateIndex);
        void loop(const LoopData& loopData);
        
    }

#endif