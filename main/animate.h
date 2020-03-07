#ifndef HEADER_ANIMATE
    #define HEADER_ANIMATE

    #include "blink-types.h"

    namespace animate {
        void pulse(Color c, byte rate);
        void pulseFace(byte face, Color c, byte rate);
        void spin(Color marker, Color background, byte rate);
        void spin(Color marker, byte rate);
        void radiate(Color marker, byte face, byte rate);
        void fillTo(const Color fill, const Color back, const byte face);
        void startFade();
        void fadeFace(const Color c, const DOUBLE_BYTE duration, const byte face);
}

#endif
