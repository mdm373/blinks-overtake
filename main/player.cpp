#include "player.h"
#include "colors.h"

namespace player {
    
    Color getColor(const byte index) {

        switch( index ) {
            
            case 0 :
                return COLOR_PLAYER1;
            
            case 1 : 
                return COLOR_PLAYER2;
            
            case 2: 
                return COLOR_PLAYER3;
            
        }
        
        return COLOR_PLAYER4;
    }
}