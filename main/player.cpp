#include "player.h"

namespace player {
    
    byte _playerEnums[PLAYER_LIMIT];
    byte _count;
    
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
        _playerEnums[_count] = enumeration;
        _count = _count + 1;
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
    
    void reset() {
        _count = 0;
    }

    byte getIndex(const byte enumeration) {
        return enumeration;
    }
}