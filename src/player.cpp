#include "player.h"

namespace player {
    #define PLAYER_LEN 4
    Color playerColors[] = {CYAN, YELLOW, MAGENTA, GREEN};
    byte playerEnums[PLAYER_LEN];
    byte count;
    byte max;

    Color getColor(const byte index) {
        return playerColors[index];
    }
    void add(const byte enumeration) {
        if(count < getMax()) {
            playerEnums[count] = enumeration;
            count = count + 1;
        }
    }
    byte getCount() {
        return count;
    }
    byte getMax() {
        return max > PLAYER_LEN ? PLAYER_LEN : max;
    }
    void setMax(const byte aMax) {
        max = aMax;
    }
    void reset() {
        count = 0;
        max = 0;
        for(int i = 0; i < PLAYER_LEN; i++) {
            playerEnums[i] = 0;
        }
    }

    byte getIndex(const byte enumeration) {
        byte count = getMax();
        for(byte i = 0; i < count; i++){
            if(playerEnums[i] == enumeration) {
                return i;
            }
        }
        return 255;
    }

    Color getMyColor(){
        return getColor(getCount());
    }
}