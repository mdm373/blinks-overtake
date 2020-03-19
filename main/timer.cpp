
#include "timer.h"
#include "blink-types.h"

namespace timer {

    twoBytes _marked = 0;
    twoBytes _duration = 0;
    TimerCallback* _onDone;
    

    void mark(twoBytes duration, TimerCallback& onDone) {
        _marked = millis();
        _duration = duration;
        _onDone = onDone;
    }
    twoBytes runningFor() {
        if(_duration == 0) {
            return 0;
        }
        twoBytes now = millis();
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