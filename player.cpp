#include "player.h"

namespace player {
    #define PLAYER_LEN = 3
    Color playerColors[] = {ORANGE, MAGENTA, CYAN};
    Color getColor(const byte index) {
        return playerColors[index];
    }
}