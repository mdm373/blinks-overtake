#include "state-player-assign.h"
#include "state-enumerate.h"
#include "game-def.h"
#include "action.h"
#include "player.h"
#include "animate.h"


namespace statePlayerAssign{
    byte neighborAssigns[FACE_COUNT];
    BlinkTime pickTime;
    byte pickFace;

    bool handleSwitchToBoard(const action::Action& action) {
        if(player::getCount() == player::getMax()) {
            stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
            return true;
        }
        if(buttonSingleClicked() && player::getCount() >= 2) {
            stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_START, .payload=(byte)millis()});
            return true;
        }
        if(action::isBroadcastRecieved(action, GAME_DEF_ACTION_START)) {
            stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
            return true;
        }
        return false;
    }

    bool handlePlayerPicked(const action::Action& action){
        if(action::isBroadcastRecieved(action, GAME_DEF_ACTION_PLAYER_PICKED)){
            player::add(action.payload);
            pickFace = FACE_COUNT;
            pickTime = 0;
            FOREACH_FACE(f){
                if(neighborAssigns[f] == action.payload) {
                    neighborAssigns[f] = STATE_ENUMERATE_MAX;
                }
            }
            return true;
        }
        if(pickFace < FACE_COUNT) {
            if(millis() - pickTime < 700) {
                return true;
            }
            if(isAlone()){
                stateCommon::handleStateChange(GAME_DEF_STATE_MOVER);
                return true;
            }

            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_PICKED, .payload=neighborAssigns[pickFace]});
            player::add(neighborAssigns[pickFace]);
            neighborAssigns[pickFace] = STATE_ENUMERATE_MAX;
            pickFace = FACE_COUNT;
            pickTime = 0;
            return true;
        }
        return false;
    }

    void updateNeighbors(){
        FOREACH_FACE(f){
            const byte value = getLastValueReceivedOnFace(f);
            if(!isValueReceivedOnFaceExpired(f) && value >= 1) {
                neighborAssigns[f] = value -1;
                continue;
            }
            if(neighborAssigns[f] == STATE_ENUMERATE_MAX) {
                continue;
            }
            
            if(pickTime == 0) {
                pickTime = millis();
                pickFace = f;
                return;
            }
        }
    }
    
    void updateView(){
        setColor(WHITE);
        const byte count = player::getCount();
        for (int i = 0; i < count; i++) {
            setColorOnFace(player::getColor(i), i);
        }
    }

    void loop(const stateCommon::LoopData& data){
        updateView();
        if (handleSwitchToBoard(data.action)){
            return;
        }
        if (handlePlayerPicked(data.action)) {
            return;
        }
        updateNeighbors();
    }

    void enter() {
        buttonSingleClicked(); //clear cache state
        pickFace = FACE_COUNT;
        pickTime = 0;
        FOREACH_FACE(f) {
            neighborAssigns[f] = STATE_ENUMERATE_MAX;
        }
        player::reset();
        player::setMax(stateEnumerate::getTotalEnumerations() - 1); // -2 players +1 total enums zero indexed
        setValueSentOnAllFaces(stateEnumerate::getMyEnumeration()+1);
    }
}