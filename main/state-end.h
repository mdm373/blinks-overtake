
#ifndef HEADER_STATE_END
    #define HEADER_STATE_END

    #include "state-common.h"

    namespace stateEnd {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
        void _totalInit();
    }
#endif