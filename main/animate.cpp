#include "animate.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace animate {
    void pulse(Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColor(dim(c, b));
    }
    void pulseOffsetFace(byte face, Color c, byte offset, byte rate, byte cap) {
        // from 127 to (127 + 63) = 127
        byte b = sin8_C( (255+ (millis()/rate) + offset) % 255 );
        if(b > cap) {
            b = cap;
        }
        setColorOnFace(dim(c, b), face);
    }
    void pulseFace(byte face, Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColorOnFace(dim(c, b), face);
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
        setColorOnFace(marker, face);
        
        pulseOffsetFace((face + 1) % FACE_COUNT                 , marker, -63, rate, 190);
        pulseOffsetFace((FACE_COUNT + face - 1) % FACE_COUNT    , marker, -63, rate, 190);
        
        pulseOffsetFace((face + 2) % FACE_COUNT                 , marker, -126, rate, 95);
        pulseOffsetFace((FACE_COUNT + face - 2) % FACE_COUNT    , marker, -126, rate, 95);
        
        pulseOffsetFace((face + 3) % FACE_COUNT    , marker, -190, rate, 45);
    }
}