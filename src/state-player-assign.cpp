#include "state-player-assign.h"
#include "state-enumerate.h"
#include "game-def.h"
#include "action.h"
#include "player.h"
#include "animate.h"


namespace statePlayerAssign{
    byte _neighborAssigns[FACE_COUNT];
    BlinkTime _pickTime;
    byte _pickFace;

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
            _pickFace = FACE_COUNT;
            _pickTime = 0;
            FOREACH_FACE(f){
                if(_neighborAssigns[f] == action.payload) {
                    _neighborAssigns[f] = STATE_ENUMERATE_MAX;
                }
            }
            return true;
        }
        if(_pickFace < FACE_COUNT) {
            if(millis() - _pickTime < 700) {
                return true;
            }
            if(isAlone()){
                stateCommon::handleStateChange(GAME_DEF_STATE_MOVER);
                return true;
            }

            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_PICKED, .payload=_neighborAssigns[_pickFace]});
            player::add(_neighborAssigns[_pickFace]);
            _neighborAssigns[_pickFace] = STATE_ENUMERATE_MAX;
            _pickFace = FACE_COUNT;
            _pickTime = 0;
            return true;
        }
        return false;
    }

    void updateNeighbors(){
        FOREACH_FACE(f){
            const byte value = getLastValueReceivedOnFace(f);
            if(!isValueReceivedOnFaceExpired(f) && value >= 1) {
                _neighborAssigns[f] = value -1;
                continue;
            }
            if(_neighborAssigns[f] == STATE_ENUMERATE_MAX) {
                continue;
            }
            
            if(_pickTime == 0) {
                _pickTime = millis();
                _pickFace = f;
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
        _pickFace = FACE_COUNT;
        _pickTime = 0;
        FOREACH_FACE(f) {
            _neighborAssigns[f] = STATE_ENUMERATE_MAX;
        }
        player::reset();
        player::setMax(stateEnumerate::getTotalEnumerations() - 1); // -2 players +1 total enums zero indexed
        setValueSentOnAllFaces(stateEnumerate::getMyEnumeration()+1);
    }
}