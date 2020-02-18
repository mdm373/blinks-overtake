#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "state-enumerate.h"
#include "game-def.h"
#include "timer.h"
namespace stateMover {
    bool _hasSent = false;
    bool _showSending = false;

    void handleViewNormalize() {
        _showSending = false;
    }
    void loop(const bool isEnter, const stateCommon::LoopData& data) {
        _hasSent = _hasSent && !isEnter;
        if(isValueReceivedOnFaceExpired(0)) {
            _hasSent = false;
        }
        
        animate::radiate(player::getColor(player::getCount()), 0, 6);
        if(!isValueReceivedOnFaceExpired(0) && !_hasSent) {
            _hasSent = true;
            _showSending = true;
            timer::mark(800, handleViewNormalize);
            action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = stateEnumerate::getMyEnumeration()}, 0);
            return;
        }        
    }
}