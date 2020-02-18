
#ifndef HEADER_TIMER
    #define HEADER_TIMER

    #include "blink-types.h"

    namespace timer {
        
        typedef void (TimerCallback)(void);

        void mark(DOUBLE_BYTE duration, TimerCallback& onDone);
        DOUBLE_BYTE runningFor();
        void loop();
        void cancel();
    }

#endif