#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "game-def.h"
#include "timer.h"
namespace stateMover {

    #define SHOW_IDLE 0
    #define SHOW_SENDING 1
    #define SHOW_DONE 2
    
    byte _showStatus = SHOW_IDLE;
    
    byte _currentPlayer;

    void handleDelayedSend() {
        action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = _currentPlayer}, 0);
    }

    void handleConnectionStatus(){
        if(isAlone()) {
            _showStatus = SHOW_IDLE;
            timer::cancel();
            return;
        }
        if(_showStatus == SHOW_IDLE && !isValueReceivedOnFaceExpired(0)) {
            _showStatus = SHOW_SENDING;
            timer::mark(50, handleDelayedSend);
        }
    }

    void handleMoveResponse(const action::Action& action){
        if(action.type == GAME_DEF_ACTION_MOVE_RESPONSE) {
            _showStatus = SHOW_DONE;
            if(action.payload > 0) {
                _currentPlayer = (_currentPlayer + 1) % player::getCount();
            }
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data) {
        if(isEnter) {
            _showStatus = SHOW_IDLE;
            _currentPlayer = 0;
        }
        handleMoveResponse(data.action);
        handleConnectionStatus();
        if(_showStatus == SHOW_SENDING) {
            animate::spin(player::getColor(_currentPlayer), 4);
            return;
        }
        animate::radiate(player::getColor(_currentPlayer), 0, 6);
    }
}