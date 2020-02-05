
#ifndef HEADER_STATE_ENUMERATE
    #define HEADER_STATE_ENUMERATE

    #include "state-common.h"

    namespace stateEnumerate {
        void loopNone(const stateCommon::LoopData& data);
        void enterNone();
        void loopPending(const stateCommon::LoopData& data);
        void loopAssigned(const stateCommon::LoopData& data);
        byte getTotalEnumerations();
        byte getMyEnumeration();
        
        #define STATE_ENUMERATE_MAX 0b01111111
    }

#endif