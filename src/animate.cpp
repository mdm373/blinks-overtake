#include "animate.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace animate {
    void pulse(Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColor(dim(c, b));
    }

    void fillTo(const Color fill, const Color back, const byte to){
        FOREACH_FACE(f) {
            setColorOnFace(f <= to ? fill : back, f);
        }
    }
    void spin(Color c, byte rate) {
        byte face = (millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f) {
            byte distance = f <= face ? (face - f) : (( FACE_COUNT  - f) + face);
            byte brightness = 255 - (distance * 51);
            setColorOnFace(dim(c, sin8_C(brightness)), f);
        }
    }

    void spin(Color marker, Color background, byte rate) {
        byte face = (millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f) {
            if( face == f) {
                setColorOnFace(marker, f);
            } else {
                setColorOnFace(background, f);
            }
        }
    }

    void radiate(Color marker, byte face, byte rate){
        pulse(marker, rate);
        setColorOnFace(marker, face);
    }
}