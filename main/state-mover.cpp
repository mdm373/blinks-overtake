#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "game-def.h"
#include "timer.h"
namespace stateMover {

    #define VIEW_IDLE 0
    #define VIEW_SENDING 1
    
    byte _viewState = VIEW_IDLE;
    byte _currentPlayer = 0;

    void handleDelayedSend() {
        action::send( (byte) GAME_DEF_ACTION_MOVE_REQUEST,  _currentPlayer , 0);
    }
    
    void handleViewNormalize() {
        _viewState = VIEW_IDLE;
    }

    void handleConnectionStatus(){
        if(isAlone()) {
            _viewState = VIEW_IDLE;
            timer::cancel();
            return;
        }
        if(_viewState == VIEW_IDLE && !isValueReceivedOnFaceExpired(0)) {
            _viewState = VIEW_SENDING;
            timer::mark(50, handleDelayedSend);
        }
    }

    void handlePlayerChange(){
        if(buttonDoubleClicked() && isAlone()) {
            _currentPlayer = (_currentPlayer + 1) % PLAYER_LIMIT;
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data) {
        if(isEnter) {
            buttonDoubleClicked();
            timer::cancel();
            _viewState = VIEW_IDLE;
            _currentPlayer = 0;
        }
        handlePlayerChange();
        handleConnectionStatus();
        if(_viewState == VIEW_SENDING) {
            animate::radiate(player::getColor(_currentPlayer), 0, 4);
            return;
        }
        setColor(OFF);
        setColorOnFace(dim(player::getColor(_currentPlayer), 255), 0);
        setColorOnFace(dim(WHITE, 120), 1);
        setColorOnFace(dim(WHITE, 120), 5);
    }
}