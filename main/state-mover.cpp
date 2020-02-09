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
    bool _hasSent;

    void updateView(){
        if(timestamp::isClear()) {
            animate::radiate(player::getMyColor(), 0, 6);
            return;
        }
        
        if(timestamp::getDuration() > 800) {
            timestamp::clear();
            return;
        }
        animate::spin(player::getMyColor(), 4);
    }

    void loop(const stateCommon::LoopData& data) {
        updateView();
        if(!timestamp::isClear()){
            return;
        }
        if(!_hasSent && !isValueReceivedOnFaceExpired(0)) {
            _hasSent = true;
            action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = stateEnumerate::getMyEnumeration()}, 0);
            timestamp::mark();
            return;
        }
        if(_hasSent && isValueReceivedOnFaceExpired(0)) {
            _hasSent = false;
        }
        
    }

    void enter() {
        _hasSent = false;
        timestamp::clear();
    }
}