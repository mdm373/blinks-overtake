#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"
#include "timer.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace stateBoard {
    
    #define FACE_ELSEWHERE FACE_COUNT + 1
    #define VIEW_STATE_NORMAL 0
    #define VIEW_STATE_ERROR 1
    #define VIEW_STATE_RADIATE 2
    
    byte _ownershipe[FACE_COUNT];
    byte _playerToFaceRequestes[PLAYER_LIMIT];
    bool _isEndInitiator;

    //reusable
    byte _moveIndex;
    byte _viewState;
    
    void handleEnter() {
        _isEndInitiator = false;
        _moveIndex = 0;
        _viewState = VIEW_STATE_NORMAL;
        buttonSingleClicked();
        for(byte i = 0; i < PLAYER_LIMIT; i++) {
            _playerToFaceRequestes[i] = FACE_COUNT;
        }
    }

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
        if(_viewState == VIEW_STATE_ERROR) {
            animate::pulse(RED, 4);
            return;
        }
        if(_viewState == VIEW_STATE_RADIATE) {
            animate::radiate(player::getColor(_moveIndex), _playerToFaceRequestes[_moveIndex], 1);
            return;
        }
        drawOwners();
    }

    void handleViewNormalize(){
        _viewState = VIEW_STATE_NORMAL;
    }
    void processPlayerRequests(const stateCommon::LoopData& data){
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_MOVE_TAKEN)) {
            _playerToFaceRequestes[data.action.payload] = FACE_ELSEWHERE;
        }
        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && timer::runningFor()  == 0) {
            _moveIndex = player::getIndex(data.action.payload);
            timer::mark(800, handleViewNormalize);
            if(_playerToFaceRequestes[_moveIndex] != FACE_COUNT || _ownershipe[data.face] != PLAYER_LIMIT) {
                _viewState = VIEW_STATE_ERROR;
                return;
            }
            _viewState = VIEW_STATE_RADIATE;
            _playerToFaceRequestes[_moveIndex] = data.face;
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_MOVE_TAKEN, .payload=_moveIndex});
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
        timer::cancel();
        stateCommon::handleStateChange(GAME_DEF_STATE_PROGRESS);
    }

    bool handleProgression(const stateCommon::LoopData& data){
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_PROGRESS)) {
            changeToProgress();
            return true;
        }
        if(action::isBroadcastRecieved(data.action, GAME_DEF_ACTION_END)) {
            timer::cancel();
            stateCommon::handleStateChange(GAME_DEF_STATE_END);
            return true;
        }
        if(buttonSingleClicked()){
            bool allEmpty = true;
            bool someEmpty = false;
            for(byte i =0; i < player::getCount(); i++) {
                bool isEmpty = _playerToFaceRequestes[i] == FACE_COUNT;
                allEmpty = allEmpty && isEmpty;
                someEmpty = someEmpty || isEmpty;
            }
            if(allEmpty) {
                action::broadcast(action::Action{.type=GAME_DEF_ACTION_END, .payload=millis()});
                _isEndInitiator = true;
                timer::cancel();
                stateCommon::handleStateChange(GAME_DEF_STATE_END);
                return false;
            }
            if(someEmpty) {
                _viewState = VIEW_STATE_ERROR;
                timer::mark(800, handleViewNormalize);
                return false;
            }
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PROGRESS, .payload = millis()});
            changeToProgress();
            return true;
        }
        return false;
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            handleEnter();
        }
        updateView();
        processPlayerRequests(data);
        handleProgression(data);
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

    bool isEndInitator(){
        return _isEndInitiator;
    }
}