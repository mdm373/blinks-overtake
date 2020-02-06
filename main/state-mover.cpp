#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "state-enumerate.h"
#include "game-def.h"
#include "timestamp.h"

namespace stateMover {
    
    //reusable
    bool _isError;

    void updateView(){
        if(timestamp::isClear()) {
            animate::radiate(player::getMyColor(), 0, 6);
            return;
        }
        
        if(timestamp::getDuration() > 800) {
            timestamp::clear();
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
        if(buttonSingleClicked() && timestamp::isClear()){
            timestamp::mark();
            if(isValueReceivedOnFaceExpired(0)){
                _isError = true;
                return;
            }
            action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = stateEnumerate::getMyEnumeration()}, 0);            
        }
    }

    void enter() {
        _isError = false;
        timestamp::clear();
        buttonSingleClicked(); //reset the cached state
    }
}