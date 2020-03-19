#include "player.h"
#include "colors.h"

namespace player {
    
    Color getColor(const byte index) {
        if(index == 0) {
            return COLOR_PLAYER1;
        }
        if(index == 1) {
            return COLOR_PLAYER2;
        }
        if(index == 2) {
            return COLOR_PLAYER3;
        }
        return COLOR_PLAYER4;
    }
}