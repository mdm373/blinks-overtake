
#ifndef HEADER_TIMER
    #define HEADER_TIMER

    #include "blink-types.h"

    namespace timer {
        
        typedef void (TimerCallback)(void);

        void mark(twoBytes duration, TimerCallback& onDone);
        twoBytes runningFor();
        void loop();
        void cancel();
    }

#endif