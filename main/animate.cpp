#include "animate.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace animate {

    DOUBLE_BYTE _fadeStart = 0;

    void pulse(Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColor(dim(c, b));
    }
    void pulseOffsetFace(byte face, Color c, byte offset, byte rate) {
        setColorOnFace(dim(c, sin8_C( (255+ (millis()/rate) + offset) % 255 )), face);
    }
    void pulseFace(byte face, Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColorOnFace(dim(c, b), face);
    }

    void fillTo(const Color fill, const Color back, const byte to){
        FOREACH_FACE(f){
            setColorOnFace(f <= to ? fill : back, f);
        }
    }

    void spin(Color c, byte rate) {
        byte face = (millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f){
            byte distance = f <= face ? (face - f) : (( FACE_COUNT  - f) + face);
            byte brightness = 255 - (distance * 51);
            setColorOnFace(dim(c, sin8_C(brightness)), f);
        }
    }

    void spin(Color marker, Color background, byte rate) {
        byte face = (millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f){
            if( face == f) {
                setColorOnFace(marker, f);
            } else {
                setColorOnFace(background, f);
            }
        }
    }

    void radiate(Color marker, byte face, byte rate){
        setColor(OFF);
        setColorOnFace(marker, face);
        pulseOffsetFace((face + 1) % FACE_COUNT                 , marker, -63, rate);
        pulseOffsetFace((FACE_COUNT + face - 1) % FACE_COUNT    , marker, -63, rate);
    }

    void startFade() {
        _fadeStart = millis();
    }

    void fadeFace(const Color c, const DOUBLE_BYTE duration, const byte face) {
        DOUBLE_BYTE periodToBright = 25500 / duration;
        setColorOnFace(dim(c,  ((millis() - _fadeStart) * periodToBright)/100), face);
    }
}