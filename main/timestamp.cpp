
#include "timestamp.h"
#include "blink-types.h"

namespace timestamp {

    DOUBLE_BYTE _marked = 0;
    
    void mark() {
        _marked = millis();
    }
    DOUBLE_BYTE getDuration() {
        if(_marked == 0) {
            return 0;
        }
        DOUBLE_BYTE now = millis();
        if(now < _marked) {
            return 0b1111111111111111;
        }
        return now - _marked;
    }
    void clear() {
        _marked = 0;
    }
    bool isClear() {
        return _marked == 0;
    }
}