#include "player.h"

namespace player {
    
    byte _playerEnums[PLAYER_LIMIT];
    byte _count;
    byte _max;

    Color getColor(const byte index) {
        if(index == 0) {
            return CYAN;
        }
        if(index == 1) {
            return YELLOW;
        }
        if(index == 2) {
            return MAGENTA;
        }
        return GREEN;
    }

    void add(const byte enumeration) {
        if(_count < getMax()) {
            _playerEnums[_count] = enumeration;
            _count = _count + 1;
        }
    }

    bool hasEnum(const byte enumeration) {
        for(byte i = 0; i < _count; i++) {
            if(_playerEnums[i] == enumeration) {
                return true;
            }
        }
        return false;
    }
    
    byte getCount() {
        return _count;
    }
    byte getMax() {
        return _max > PLAYER_LIMIT ? PLAYER_LIMIT : _max;
    }
    void setMax(const byte max) {
        _max = max;
    }
    void reset() {
        _count = 0;
        _max = 0;
        for(int i = 0; i < PLAYER_LIMIT; i++) {
            _playerEnums[i] = 0;
        }
    }

    byte getIndex(const byte enumeration) {
        byte count = getMax();
        for(byte i = 0; i < count; i++){
            if(_playerEnums[i] == enumeration) {
                return i;
            }
        }
        return 255;
    }

    Color getMyColor(){
        return getColor(getCount());
    }
}