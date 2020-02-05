#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"

namespace stateBoard {
    
    byte moveFace;
    BlinkTime moveTime;
    byte currentPlayer;
    byte moveIndex;

    void loop(const stateCommon::LoopData& data){
        if(moveTime == 0){
            setColor(WHITE);
        } else {
            if(millis() - moveTime > 800) {
                moveTime = 0;
            }
            animate::spin(player::getColor(moveIndex), 4);
        }

        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && moveTime == 0) {
            moveTime = millis();
            moveFace = data.face;
            moveIndex = player::getIndex(data.action.payload);
        }
    }
    void enter(){
        moveFace = FACE_COUNT;
        moveTime = 0;
        moveIndex = 255;
        currentPlayer = 0;
    }
}