#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"
#include "timestamp.h"

namespace stateBoard {
    
    //reusable
    byte _moveFace;
    byte _currentPlayer;
    byte _moveIndex;

    void loop(const stateCommon::LoopData& data){
        if(timestamp::isClear()){
            setColor(WHITE);
        } else {
            if(timestamp::getDuration() > 800) {
                timestamp::clear();
            }
            animate::spin(player::getColor(_moveIndex), 4);
        }

        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && timestamp::isClear()) {
            timestamp::mark();
            _moveFace = data.face;
            _moveIndex = player::getIndex(data.action.payload);
        }
    }
    void enter(){
        _moveFace = FACE_COUNT;
        timestamp::clear();
        _moveIndex = 255;
        _currentPlayer = 0;
    }
}