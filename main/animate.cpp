#include "animate.h"
#include "math.h"
#include <Arduino.h>

#pragma clang diagnostic ignored "-Wnarrowing"

namespace animate {

    twoBytes _fadeStart = 0;

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

    void startAnim() {
        _fadeStart = millis();
    }

    void animFadeFace(const Color c, const twoBytes duration, const byte func, const byte face) {
        twoBytes periodToBright = 25500 / duration;
        twoBytes elapsed = millis() - _fadeStart;
        byte factor = (elapsed * periodToBright) / 100;
        if(func == ANIMATE_FUNCTION_IN) {
            factor = min( factor, 255);
        } else {
            factor = max(255 - factor, 0);
        }
        setColorOnFace(dim(c, factor), face);
    }

    byte getComponent(byte from, byte to, twoBytes duration, twoBytes elapsed) {
        bool isDown = to < from;
        twoBytes period = max(to, from) - min(to, from);
        twoBytes delta = (((period*1000) / duration) * elapsed) / 1000;
        return isDown ? from - delta : from + delta;
    }

    void animTransitionFace(const Color from, const Color to, const twoBytes duration, const byte face) {
        twoBytes elapsed = millis() - _fadeStart;
        byte r = getComponent(from.r, to.r, duration, elapsed);
        byte g = getComponent(from.g, to.g, duration, elapsed);
        byte b = getComponent(from.b, to.b, duration, elapsed);
        setColorOnFace(MAKECOLOR_5BIT_RGB(r, g, b), face);
    }
}