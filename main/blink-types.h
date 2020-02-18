#ifndef HEADER_BLINK_TYPES
    #define HEADER_BLINK_TYPES
    #include <stdint.h>

    #ifdef TEST
        #include "../test/blinklib-mock.h"
    #else
        #include <ArduinoTypes.h>
        #include <blinklib.h>
    #endif
    
    #define DOUBLE_BYTE uint16_t
    
#endif