

#ifndef HEADER_PLAYER
#define HEADER_PLAYER

#include "blink-types.h"

namespace player {
    #define PLAYER_LIMIT 4
    Color getColor(const byte index);
    void add(const byte enumeration);
    byte getCount();
    byte getMax();
    byte getIndex(const byte enumeration);
    void setMax(const byte max);
    void reset();
    Color getMyColor();
}
#endif