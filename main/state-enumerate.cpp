#include "state-enumerate.h"
#include "animate.h"
#include "game-def.h"
#include "action.h"
#include "global-events.h"
#include "log.h"

namespace stateEnumerate {
    
    byte _enumeration;
    byte _totalEnumerations;
    byte _incomingFace;
    byte _outgoingFace;
    byte _outgoingEnumeration;

    bool respondNoAssign(const stateCommon::LoopData& data){
        if (data.action.type == GAME_DEF_ACTION_ENUMERATE_REQUEST) {
            action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=0}, data.face);
            return true;
        }
        return false;
    }

    void sendBack(byte assignedOut) {
        if(_incomingFace < FACE_COUNT) {
            const bool sent = action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=assignedOut}, _incomingFace);
            if(!sent) {
                globalEvents::changeAllToFail();
                return;
            }
            stateCommon::handleStateChange(GAME_DEF_STATE_ENUM_ASGN);
            return;
        }

        if(assignedOut < 3) {
            globalEvents::changeAllToFail();
            return;
        }

        _totalEnumerations = assignedOut;
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_ASSGN_PLAYERS, .payload = _totalEnumerations});
        stateCommon::handleStateChange(GAME_DEF_STATE_PLAYER_ASSIGN);
    }

    void sendAroundThenBack() {
        bool sent = false;
        while(sent == false && _outgoingFace < FACE_COUNT) {
            if(_outgoingFace == _incomingFace) {
                _outgoingFace = _outgoingFace + 1;
                continue;    
            }
            sent = action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_REQUEST, .payload=_outgoingEnumeration}, _outgoingFace);
            if(!sent){
                _outgoingFace = _outgoingFace + 1;
            }
        }
        if(!sent) {
            sendBack(_outgoingEnumeration-1);
            return;
        }
        if(stateCommon::getCurrent() != GAME_DEF_STATE_ENUM_PEND){
            stateCommon::handleStateChange(GAME_DEF_STATE_ENUM_PEND);
        }
    }
    
    void loopNone(const stateCommon::LoopData& data){
        animate::pulse(WHITE, 8);
        
        const bool wasNotClicked = !buttonSingleClicked();
        if (wasNotClicked && data.action.type != GAME_DEF_ACTION_ENUMERATE_REQUEST) {
            return;
        }
        
        _incomingFace = FACE_COUNT;
        _enumeration = 0;
        if (wasNotClicked){
            _incomingFace = data.face;
            _enumeration = data.action.payload;
        }
        if (_enumeration > STATE_ENUMERATE_MAX) {
            globalEvents::changeAllToFail();
            return;
        }
        
        _outgoingFace = 0;
        _outgoingEnumeration = _enumeration + 1;
        sendAroundThenBack();
    }

    void loopPending(const stateCommon::LoopData& data){
        animate::spin(WHITE, 4);
        if (respondNoAssign(data)) {
            return;
        }

        if (data.action.type != GAME_DEF_ACTION_ENUMERATE_RESPONSE) {
            return;
        }

        if( data.face != _outgoingFace) {
            globalEvents::changeAllToFail();
            return;
        }
        
        _outgoingFace = _outgoingFace + 1;
        if (data.action.payload >= _outgoingEnumeration) {
            _outgoingEnumeration = data.action.payload+1;
        }
        sendAroundThenBack();

    }

    void loopAssigned(const stateCommon::LoopData& data){
        if (respondNoAssign(data)) {
            return;
        }
        
        if (action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_ASSGN_PLAYERS)) {
            _totalEnumerations = data.action.payload;
            stateCommon::handleStateChange(GAME_DEF_STATE_PLAYER_ASSIGN);
        }
    }

    void enterNone() {
        buttonSingleClicked(); //clear the buffer
        _enumeration = 0;
        _totalEnumerations = 0;
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
        _outgoingEnumeration = 0;
    }

    byte getTotalEnumerations(){
        return _totalEnumerations;
    }

    byte getMyEnumeration(){
        return _enumeration;
    }

}