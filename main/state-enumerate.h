
#ifndef HEADER_STATE_ENUMERATE
    #define HEADER_STATE_ENUMERATE

    #include "state-common.h"

    namespace stateEnumerate {
        void loop(const stateCommon::LoopData& data);
        void enter();
        byte getTotalEnumerations();
        byte getMyEnumeration();
        
        #define STATE_ENUMERATE_MAX 0b01111111
    }

#endif