
#ifndef HEADER_STATE_PROGRESS
    #define HEADER_STATE_PROGRESS

    #include "state-common.h"

    namespace stateProgress {
        void loop(const stateCommon::LoopData& data);
        void enter();
    }

#endif