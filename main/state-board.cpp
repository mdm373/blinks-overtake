#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"
#include "timestamp.h"
#include "state-enumerate.h"
#pragma clang diagnostic ignored "-Wnarrowing"

namespace stateBoard {
    
    #define FACE_ELSEWHERE FACE_COUNT + 1
    
    byte _ownershipe[FACE_COUNT];
    byte _playerToFaceRequestes[PLAYER_LIMIT];

    //reusable
    byte _moveIndex;
    bool _isError;
    
    void updateOffOwners(){
        FOREACH_FACE(f){
            setValueSentOnFace(_ownershipe[f]+1, f);
        }
    }

    void drawOwners() {
        FOREACH_FACE(f) {
            if(_ownershipe[f] == PLAYER_LIMIT) {
                setColorOnFace(OFF, f);
                continue;
            }
            setColorOnFace(player::getColor(_ownershipe[f]), f);
        }
    }

    void updateView() {
        if (!timestamp::isClear()) {
            if(timestamp::getDuration() < 800) {
                if(_isError) {
                    animate::pulse(RED, 4);
                    return;
                }
                animate::radiate(player::getColor(_moveIndex), _playerToFaceRequestes[_moveIndex], 1);
                return;
            }
            _isError = false;
            timestamp::clear();
        }
        drawOwners();
    }

    void processPlayerRequests(const stateCommon::LoopData& data){
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_MOVE_TAKEN)) {
            _playerToFaceRequestes[data.action.payload] = FACE_ELSEWHERE;
        }
        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && timestamp::isClear()) {
            _moveIndex = player::getIndex(data.action.payload);
            if(_playerToFaceRequestes[_moveIndex] != FACE_COUNT) {
                _isError = true;
                timestamp::mark();
                return;
            }
            if(_ownershipe[data.face] != PLAYER_LIMIT){
                _isError = true;
                timestamp::mark();
                return;
            }
            _playerToFaceRequestes[_moveIndex] = data.face;
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_MOVE_TAKEN, .payload=_moveIndex});
            timestamp::mark();
        }
    }

    void changeToProgress(){
        byte faceBuffer[FACE_COUNT];
        FOREACH_FACE(f){
            byte count = getRequestsForFace(f, &(faceBuffer[0]));
            if(count == 1) {
                applyOwner(f, faceBuffer[0]);
            }
        }
        stateCommon::handleStateChange(GAME_DEF_STATE_PROGRESS);
    }

    bool handleProgression(const stateCommon::LoopData& data){
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_PROGRESS)) {
            changeToProgress();
            return true;
        }
        if(buttonSingleClicked()){
            const byte playerCount = player::getCount();
            for(byte i =0; i < playerCount; i++) {
                if(_playerToFaceRequestes[i] == FACE_COUNT) {
                    _isError = true;
                    timestamp::mark();
                    return false;
                }
            }
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PROGRESS, .payload = millis()});
            changeToProgress();
            return true;
        }
        return false;
    }

    void loop(const stateCommon::LoopData& data){
        updateView();
        processPlayerRequests(data);
        handleProgression(data);
    }

    void enter(){
        _isError = false;
        _moveIndex = PLAYER_LIMIT;
        timestamp::clear();
        buttonSingleClicked();
        for(byte i = 0; i < PLAYER_LIMIT; i++) {
            _playerToFaceRequestes[i] = FACE_COUNT;
        }
    }

    void reset() {
        FOREACH_FACE(f){
            _ownershipe[f] = PLAYER_LIMIT;
        }
    }

    byte getRequestsForFace(const byte face, byte* playerBuffer) {
        byte count = 0;
        for(byte i = 0; i < PLAYER_LIMIT; i++) {
            if(_playerToFaceRequestes[i] == face){
                playerBuffer[count] = i;
                count++;
            }
        }
        return count;
    }

    void applyOwner(const byte face, const byte playerIndex) {
        _ownershipe[face] = playerIndex;
    }

    byte getOwnershipe(const byte face){
        return _ownershipe[face];
    }

    byte getOffOwnershipe(const byte face) {
        if(isValueReceivedOnFaceExpired(face)) {
            return PLAYER_LIMIT;
        }
        const byte value = getLastValueReceivedOnFace(face);
        if(value == 0) {
            return PLAYER_LIMIT;
        }
        return value - 1;
    }
}