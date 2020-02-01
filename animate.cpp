#include "animate.h"

namespace animate {
    static void pulse(Color c, byte rate) {
        byte b = sin8_C(millis()/rate);
        setColor(dim(c, b));
    }

    static void spin(Color c, byte rate) {
        byte face = (byte)(millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f) {
            byte distance = f <= face ? (face - f) : (( FACE_COUNT  - f) + face);
            byte brightness = 255 - (distance * 51);
            setFaceColor(f, dim(c, sin8_C(brightness)));
        }
    }

    static void spin(Color marker, Color background, byte rate) {
        byte face = (byte)(millis()/rate) / 42; //roughly 255 / faceCount 
        FOREACH_FACE(f) {
            if( face == f) {
                setFaceColor(f, marker);
            } else {
                setFaceColor(f, background);
            }
        }
    }

    static void radiate(Color marker, byte face, byte rate){
        pulse(marker, rate);
        setColorOnFace(marker, face);
    }
}