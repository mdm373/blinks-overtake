#include "state-enumerate.h"
#include "animate.h"
#include "game-def.h"
#include "action.h"
#include "global-events.h"
#include "log.h"

namespace stateEnumerate {
    
    byte enumeration;
    byte totalEnumerations;

    byte incomingFace;
    byte outgoingFace;
    byte outgoingEnumeration;

    bool respondNoAssign(const stateCommon::LoopData& data){
        if (data.action.type == GAME_DEF_ACTION_ENUMERATE_REQUEST) {
            action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=0}, data.face);
            return true;
        }
        return false;
    }

    void sendBack(byte assignedOut) {
        if(incomingFace < FACE_COUNT) {
            const bool sent = action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=assignedOut}, incomingFace);
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

        totalEnumerations = assignedOut;
        action::broadcast(action::Action{.type=GAME_DEF_ACTION_TO_STATE_PLAYER_ASSIGN, .payload = totalEnumerations});
        stateCommon::handleStateChange(GAME_DEF_STATE_PLAYER_ASSIGN);
    }

    void sendAroundThenBack() {
        bool sent = false;
        while(sent == false && outgoingFace < FACE_COUNT) {
            if(outgoingFace == incomingFace) {
                outgoingFace = outgoingFace + 1;
                continue;    
            }
            sent = action::send(action::Action{.type = GAME_DEF_ACTION_ENUMERATE_REQUEST, .payload=outgoingEnumeration}, outgoingFace);
            if(!sent){
                outgoingFace = outgoingFace + 1;
            }
        }
        if(!sent) {
            sendBack(outgoingEnumeration-1);
            return;
        }
        if(stateCommon::getCurrent() != GAME_DEF_STATE_ENUM_PEND){
            stateCommon::handleStateChange(GAME_DEF_STATE_ENUM_PEND);
        }
    }
    
    void loopNone(const stateCommon::LoopData& data){
        animate::pulse(WHITE, 8);
        if(!buttonSingleClicked() && data.action.type != GAME_DEF_ACTION_ENUMERATE_REQUEST) {
            return;
        }
        byte incomingEnumeration = 0;
        incomingFace = FACE_COUNT;
        if(!buttonSingleClicked()){
            incomingFace = data.face;
            incomingEnumeration = data.action.payload;
        }
        if(incomingEnumeration > STATE_ENUMERATE_MAX) {
            globalEvents::changeAllToFail();
            return;
        }
        enumeration = incomingEnumeration;
        outgoingFace = 0;
        outgoingEnumeration = incomingEnumeration+1;
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

        if( data.face != outgoingFace) {
            globalEvents::changeAllToFail();
            return;
        }
        
        outgoingFace = outgoingFace + 1;
        if (data.action.payload >= outgoingEnumeration) {
            outgoingEnumeration = data.action.payload+1;
        }
        sendAroundThenBack();

    }

    void loopAssigned(const stateCommon::LoopData& data){
        if( respondNoAssign(data)) {
            return;
        }
        
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_TO_STATE_PLAYER_ASSIGN)) {
            totalEnumerations = data.action.payload;
            stateCommon::handleStateChange(GAME_DEF_STATE_PLAYER_ASSIGN);
        }
    }

    void enterNone() {
        buttonSingleClicked(); //clear the buffer
        enumeration = 0;
        totalEnumerations = 0;
    }

    byte getTotalEnumerations(){
        return totalEnumerations;
    }

    byte getMyEnumeration(){
        return enumeration;
    }

}