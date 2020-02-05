#include "player.h"

namespace player {
    
    const Color _playerColors[] = {CYAN, YELLOW, MAGENTA, GREEN};
    
    byte _playerEnums[PLAYER_LIMIT];
    byte _count;
    byte _max;

    Color getColor(const byte index) {
        return _playerColors[index];
    }
    void add(const byte enumeration) {
        if(_count < getMax()) {
            _playerEnums[_count] = enumeration;
            _count = _count + 1;
        }
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