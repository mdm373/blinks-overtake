#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"

namespace stateBoard {
    
    BlinkTime _moveTime;
    byte _moveFace;
    byte _currentPlayer;
    byte _moveIndex;

    void loop(const stateCommon::LoopData& data){
        if(_moveTime == 0){
            setColor(WHITE);
        } else {
            if(millis() - _moveTime > 800) {
                _moveTime = 0;
            }
            animate::spin(player::getColor(_moveIndex), 4);
        }

        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && _moveTime == 0) {
            _moveTime = millis();
            _moveFace = data.face;
            _moveIndex = player::getIndex(data.action.payload);
        }
    }
    void enter(){
        _moveFace = FACE_COUNT;
        _moveTime = 0;
        _moveIndex = 255;
        _currentPlayer = 0;
    }
}