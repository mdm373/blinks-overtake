
#include "timer.h"
#include "blink-types.h"

namespace timer {

    DOUBLE_BYTE _marked = 0;
    DOUBLE_BYTE _duration = 0;
    TimerCallback* _onDone;
    

    void mark(DOUBLE_BYTE duration, TimerCallback& onDone) {
        _marked = millis();
        _duration = duration;
        _onDone = onDone;
    }
    DOUBLE_BYTE runningFor() {
        if(_duration == 0) {
            return 0;
        }
        DOUBLE_BYTE now = millis();
        if(now < _marked) {
            return 0b1111111111111111;
        }
        return now - _marked;
    }
    void loop() {
        if(_duration > 0 && runningFor() >= _duration) {
            _duration = 0;
            _onDone();
        }
    }
    
    void cancel() {
        _duration = 0;
    }
}