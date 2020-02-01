#include <ArduinoTypes.h>
#include <blinklib.h>

#ifndef HEADER_ANIMATE
#define HEADER_ANIMATE

namespace animate {
    void pulse(Color c, byte rate);
    void spin(Color marker, Color background, byte rate);
    void spin(Color marker, byte rate);
    void radiate(Color marker, byte face, byte rate);
}

#endif
