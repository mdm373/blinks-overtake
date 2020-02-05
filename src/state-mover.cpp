#include "blink-types.h"
#include "state-mover.h"
#include "animate.h"
#include "player.h"
#include "action.h"
#include "state-enumerate.h"
#include "game-def.h"
namespace stateMover {
    
    BlinkTime indcStart;
    bool isError;

    void updateView(){
        if(indcStart == 0) {
            animate::radiate(player::getMyColor(), 0, 6);
            return;
        }
        
        if(millis() - indcStart > 800) {
            indcStart = 0;
            isError = false;
            return;
        }

        if(isError) {
            animate::pulse(RED, 4);
            return;
        }

        animate::spin(player::getMyColor(), 4);
    }

    void loop(const stateCommon::LoopData& data) {
        updateView();
        if(buttonSingleClicked() && indcStart == 0){
            indcStart = millis();
            if(isValueReceivedOnFaceExpired(0)){
                isError = true;
                return;
            }
            action::send(action::Action{.type=GAME_DEF_ACTION_MOVE_REQUEST, .payload = stateEnumerate::getMyEnumeration()}, 0);            
        }
    }

    void enter() {
        isError = false;
        indcStart = 0;
        buttonSingleClicked();
    }
}