#include "blinklib-mock.h"
#include "iostream"

unsigned long millis() {
    return 137;
}

byte sin8_C( byte theta) {
    return theta;
}

Color dim( Color color, byte brightness) {
    return (color + "-").append(std::to_string(brightness));
}

void setColor( Color newColor) {
    std::cout << "[setColor] color:" << newColor << std::endl;
}

void setColorOnFace( Color newColor , byte face ) {
    std::cout << "[setColorOnFace] face:" << std::to_string(face) << ", color:" << newColor<< std::endl;
}


bool faceExpirations[FACE_COUNT];
byte incomingValues[FACE_COUNT];
bool faceChanges[FACE_COUNT];
byte faceData[FACE_COUNT][IR_DATAGRAM_LEN];
byte faceDataLen[FACE_COUNT];
int clickCount;
bool iAmAlone;

bool isAlone() {
    return iAmAlone;
}

bool isValueReceivedOnFaceExpired(byte face) {
    return faceExpirations[face];
}

void sendDatagramOnFace( const byte *data, byte len , byte face ) {
    std::string asString = "";
    for(int i =0; i < len; i++) {
        asString = asString + std::to_string(data[i]);
        if(i < len-1) {
            asString = asString + "-";
        }
    }
    std::cout << "[sendDatagramOnFace] face:" + std::to_string(face) + ", data:" + asString  << std::endl;
}

void markDatagramReadOnFace(byte face) {
    std::cout << "[markDatagramReadOnFace] face:" + std::to_string(face)  <<std::endl;
    faceDataLen[face] = 0;
}

const byte getDatagramLengthOnFace(byte face) {
    return faceDataLen[face];
}

const byte* getDatagramOnFace(byte face) {
    return &(faceData[face][0]);
}

bool isDatagramReadyOnFace(byte face) {
    return faceDataLen[face] > 0;
}

bool buttonDoubleClicked() {
    return clickCount == 2;
}
bool buttonSingleClicked() {
    return clickCount == 1;
}

bool didValueOnFaceChange(const byte face) {
    return faceChanges[face];
}

void setValueSentOnFace(const byte value, const byte face) {
    std::cout << "[setValueSentOnFace] face:" << face << ", value:" << value << std::endl;
}
void setValueSentOnAllFaces(const byte value) {
    std::cout << "[setValueSentOnAllFaces] value:" << value << std::endl;
}
byte getLastValueReceivedOnFace(byte face) {
    return incomingValues[face];
}

namespace blinklibMock {
    byte datagramPostBuffer[IR_DATAGRAM_LEN];
    
    void triggerAlone(){
        iAmAlone = true;
    }
    void click() {
        clickCount = clickCount + 1;
    }
        
    void resetMocks() {
        iAmAlone = false;
        clickCount = 0;
        FOREACH_FACE(f){
            incomingValues[f] = 0;
            faceExpirations[f] = false;
            faceChanges[f] = false;
        }
    }

    void triggerChangeOnFace(const byte face) {
        faceChanges[face] = true;
    }

    void expireValueOnFace(byte face) {
        faceExpirations[face] = true;
    }

    void expireValuesOnAllFaces() {
        FOREACH_FACE(f){
            faceExpirations[f] = true;
        }
    }

    void expireValuesOnAllFacesExcept(byte face) {
        FOREACH_FACE(f){
            if(f == face) continue;
            faceExpirations[f] = true;
        }
    }

    void expireValuesOnAllFacesExceptBoth(byte face, byte other) {
        FOREACH_FACE(f){
            if(f == face || f == other) continue;
            faceExpirations[f] = true;
        }
    }

    void postDatagramOnFace(byte face, byte length) {
        for(int i = 0; i < length; i++) {
            faceData[face][i] = datagramPostBuffer[i];
        }
        faceDataLen[face] = length;
    }
    
    void setIncomingFaceValue(const byte face, const byte value) {
        incomingValues[face] = value;
    }
}