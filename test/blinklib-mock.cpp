#include "blinklib-mock.h"
#include "iostream"


bool _faceExpirations[FACE_COUNT];
byte _incomingValues[FACE_COUNT];
bool _faceChanges[FACE_COUNT];
byte _faceData[FACE_COUNT][IR_DATAGRAM_LEN];
byte _faceDataLen[FACE_COUNT];
int _clickCount;
bool _iAmAlone;
bool _verbose;
long _millis;
byte _sin8;

unsigned long millis() {
    return _millis;
}

byte sin8_C( byte theta) {
    return _sin8;
}

Color dim( Color color, byte brightness) {
    return (color + "-").append(std::to_string(brightness));
}

void setColor( Color newColor) {
    if(_verbose){
        std::cout << "[setColor] color:" << newColor << std::endl;
    }
}

void setColorOnFace( Color newColor , byte face ) {
    if(_verbose){
        std::cout << "[setColorOnFace] face:" << std::to_string(face) << ", color:" << newColor<< std::endl;
    }
}

bool isAlone() {
    return _iAmAlone;
}

bool isValueReceivedOnFaceExpired(byte face) {
    return _faceExpirations[face];
}

void sendDatagramOnFace( const byte *data, byte len , byte face ) {
    std::string asString = "";
    for(int i =0; i < len; i++) {
        asString = asString + std::to_string(data[i]);
        if(i < len-1) {
            asString = asString + "-";
        }
    }
    if(_verbose){
        std::cout << "[sendDatagramOnFace] face:" + std::to_string(face) + ", data:" + asString  << std::endl;
    }
}

void markDatagramReadOnFace(byte face) {
    if(_verbose) {
        std::cout << "[markDatagramReadOnFace] face:" + std::to_string(face)  <<std::endl;
    }
    _faceDataLen[face] = 0;
}

const byte getDatagramLengthOnFace(byte face) {
    return _faceDataLen[face];
}

const byte* getDatagramOnFace(byte face) {
    return &(_faceData[face][0]);
}

bool isDatagramReadyOnFace(byte face) {
    return _faceDataLen[face] > 0;
}

bool buttonDoubleClicked() {
    byte clickCount = _clickCount;
    _clickCount = 0;
    return clickCount == 2;
}
bool buttonSingleClicked() {
    byte clickCount = _clickCount;
    _clickCount = 0;
    return clickCount == 1;
}

bool didValueOnFaceChange(const byte face) {
    return _faceChanges[face];
}

void setValueSentOnFace(const byte value, const byte face) {
    if (_verbose) {
        std::cout << "[setValueSentOnFace] face:" << face << ", value:" << value << std::endl;
    }
}
void setValueSentOnAllFaces(const byte value) {
    if(_verbose){
        std::cout << "[setValueSentOnAllFaces] value:" << value << std::endl;
    }
}
byte getLastValueReceivedOnFace(byte face) {
    return _incomingValues[face];
}

namespace blinklibMock {
    byte datagramPostBuffer[IR_DATAGRAM_LEN];

    void setVerbose(bool isVerbose) {
        _verbose = isVerbose;
    }
    
    void setAlone(bool isAlone){
        _iAmAlone = _iAmAlone = isAlone;
    }
    void click() {
        _clickCount = _clickCount + 1;
    }
        
    void resetMocks() {
        _iAmAlone = false;
        _clickCount = 0;
        FOREACH_FACE(f){
            _incomingValues[f] = 0;
            _faceExpirations[f] = false;
            _faceChanges[f] = false;
        }
    }

    void triggerChangeOnFace(const byte face) {
        _faceChanges[face] = true;
    }

    void expireValueOnFace(byte face) {
        _faceExpirations[face] = true;
    }

    void expireValuesOnAllFaces() {
        FOREACH_FACE(f){
            _faceExpirations[f] = true;
        }
    }

    void expireValuesOnAllFacesExcept(byte face) {
        FOREACH_FACE(f){
            if(f == face) continue;
            _faceExpirations[f] = true;
        }
    }

    void expireValuesOnAllFacesExceptBoth(byte face, byte other) {
        FOREACH_FACE(f){
            if(f == face || f == other) continue;
            _faceExpirations[f] = true;
        }
    }

    void postDatagramOnFace(byte face, byte length) {
        for(int i = 0; i < length; i++) {
            _faceData[face][i] = datagramPostBuffer[i];
        }
        _faceDataLen[face] = length;
    }
    
    void setIncomingFaceValue(const byte face, const byte value) {
        _incomingValues[face] = value;
    }
}