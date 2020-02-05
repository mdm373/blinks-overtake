
#ifndef HEADER_TIMESTAMP
    #define HEADER_TIMESTAMP

    #include "blink-types.h"

    namespace timestamp {
        
        void mark();
        DOUBLE_BYTE getDuration();
        void clear();
        bool isClear();
    }

#endif