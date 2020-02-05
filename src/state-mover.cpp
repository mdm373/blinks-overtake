#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "state-enumerate.h"
#include "game-def.h"

namespace stateMover {
    
    BlinkTime _indcStart;
    bool _isError;

    void updateView(){
        if(_indcStart == 0) {
            animate::radiate(player::getMyColor(), 0, 6);
            return;
        }
        
        if(millis() - _indcStart > 800) {
            _indcStart = 0;
            _isError = false;
            return;
        }

        if(_isError) {
            animate::pulse(RED, 4);
            return;
        }

        animate::spin(player::getMyColor(), 4);
    }

    void loop(const stateCommon::LoopData& data) {
        updateView();
        if(buttonSingleClicked() && _indcStart == 0){
            _indcStart = millis();
            if(isValueReceivedOnFaceExpired(0)){
                _isError = true;
                return;
            }
            action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = stateEnumerate::getMyEnumeration()}, 0);            
        }
    }

    void enter() {
        _isError = false;
        _indcStart = 0;
        buttonSingleClicked(); //reset the cached state
    }
}