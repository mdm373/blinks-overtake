#ifndef HEADER_GLOBAL_EVENTS
    #define HEADER_GLOBAL_EVENTS

    #include "state-common.h"
    
    namespace globalEvents {
        bool isHandled(const stateCommon::LoopData& data);
        bool checkWake();
    }

#endif