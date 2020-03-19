#ifndef HEADER_ANIMATE
    #define HEADER_ANIMATE

    #include "blink-types.h"

    namespace animate {

        #define ANIMATE_FUNCTION_IN 0
        #define ANIMATE_FUNCTION_OUT 1
        #define ANIMATE_FUNCTION_COUNT 2
        
        void pulse(Color c, byte rate);
        void pulseFace(byte face, Color c, byte rate);
        void spin(Color marker, Color background, byte rate);
        void spin(Color marker, byte rate);
        void radiate(Color marker, byte face, byte rate);
        void fillTo(const Color fill, const Color back, const byte face);
        void startAnim();
        void animFadeFace(const Color c, const twoBytes duration, const byte function, const byte face);
        void animTransitionFace(const Color from, const Color to, const twoBytes duration, const byte face);
}

#endif
