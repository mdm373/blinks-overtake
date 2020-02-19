#include "state-player-assign.h"
#include "state-enumerate.h"
#include "game-def.h"
#include "action.h"
#include "player.h"
#include "animate.h"
#include "timer.h"
#include "state-board.h"
namespace statePlayerAssign{
    
    byte _neighborAssigns[FACE_COUNT];
    byte _pickFace;
    bool isError;


    void handleToBoardState(){
        stateBoard::reset();
        timer::cancel();
        stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
    }

    void stopError() {
        isError = false;
    }
    void handleSwitchToBoard(const action::Action& action) {
        if(player::getCount() == player::getMax()) {
            handleToBoardState();
            return;
        }
        if(buttonSingleClicked()) {
            if(player::getCount() < 2) {
                timer::mark(700, stopError);
                isError = true;
                return;
            }
            handleToBoardState();
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_START, .payload=(byte)millis()});
            return;
        }
        if(action::isBroadcastRecieved(action, GAME_DEF_ACTION_START)) {
            handleToBoardState();
            return;
        }
    }

    void handlePlayerPicked(const action::Action& action){
        if(action::isBroadcastRecieved(action, GAME_DEF_ACTION_PLAYER_PICKED)){
            player::add(action.payload);
            _pickFace = FACE_COUNT;
            timer::cancel();
            FOREACH_FACE(f){
                if(_neighborAssigns[f] == action.payload) {
                    _neighborAssigns[f] = STATE_ENUMERATE_MAX;
                }
            }
        }
    }

    void handleDelayedPick() {
        if(isAlone()){
            stateCommon::handleStateChange(GAME_DEF_STATE_MOVER);
            return;
        }
        if(player::hasEnum(_neighborAssigns[_pickFace])){
            _neighborAssigns[_pickFace] = STATE_ENUMERATE_MAX;
            _pickFace = FACE_COUNT;
            return;
        }
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_PICKED, .payload=_neighborAssigns[_pickFace]});
        player::add(_neighborAssigns[_pickFace]);
        _neighborAssigns[_pickFace] = STATE_ENUMERATE_MAX;
        _pickFace = FACE_COUNT;
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
            
            if(timer::runningFor() == 0) {
                _pickFace = f;
                timer::mark(700, handleDelayedPick);
                return;
            }
        }
    }
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            isError = false;
            buttonSingleClicked(); //clear cache state
            _pickFace = FACE_COUNT;
            FOREACH_FACE(f) {
                _neighborAssigns[f] = STATE_ENUMERATE_MAX;
            }
            player::reset();
            player::setMax(stateEnumerate::getTotalEnumerations() - 1); // -2 players +1 total enums zero indexed
            setValueSentOnAllFaces(stateEnumerate::getMyEnumeration()+1);
        }

        if(isError) {
            animate::pulse(RED, 4);
        } else {
            setColor(player::getColor(player::getCount()));
        }

        handleSwitchToBoard(data.action);
        handlePlayerPicked(data.action);
        updateNeighbors();
    }
}