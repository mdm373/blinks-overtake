#ifndef HEADER_BLINK_TYPES
#define HEADER_BLINK_TYPES

#define BlinkTime unsigned long

#ifdef TEST
    #include "../test/blinklib-mock.h"
#else
    #include <ArduinoTypes.h>
    #include <blinklib.h>
#endif

#endif