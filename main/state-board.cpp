#include "state-board.h"
#include "animate.h"
#include "game-def.h"
#include "player.h"
#include "timer.h"
#include "colors.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace stateBoard {
    
    #define FACE_ELSEWHERE FACE_COUNT + 1
    #define VIEW_STATE_NORMAL 0
    #define VIEW_STATE_ERROR 1
    #define VIEW_STATE_RADIATE 2
    
    byte _ownership[FACE_COUNT];
    byte _playerToFaceRequests[PLAYER_LIMIT];
    byte _lastFace;
    bool _isEndInitiator;
    byte _moveIndex;
    byte _viewState;
    bool _isTransitioningToMover;
    
    void handleEnter() {
        _isTransitioningToMover = false;
        _isEndInitiator = false;
        _moveIndex = 0;
        _viewState = VIEW_STATE_NORMAL;
        buttonSingleClicked();
        buttonDoubleClicked();
        buttonMultiClicked();
        for(byte i = 0; i < PLAYER_LIMIT; i++) {
            _playerToFaceRequests[i] = FACE_COUNT;
        }
        timer::cancel();
    }

    void updateOffOwners(){
        FOREACH_FACE(f){
            setValueSentOnFace(_ownership[f]+1, f);
        }
    }

    void drawOwners() {
        FOREACH_FACE(f) {
            if(_ownership[f] == PLAYER_LIMIT) {
                setColorOnFace(COLOR_FIELD, f);
                continue;
            }
            setColorOnFace(player::getColor(_ownership[f]), f);
        }
    }

    void updateView() {
        if(_viewState == VIEW_STATE_ERROR) {
            animate::pulse(RED, 4);
            return;
        }
        if(_viewState == VIEW_STATE_RADIATE) {
            animate::radiate(player::getColor(_moveIndex), _playerToFaceRequests[_moveIndex], 1);
            return;
        }
        drawOwners();
    }

    void handleViewNormalize(){
        _viewState = VIEW_STATE_NORMAL;
    }
    
    void processPlayerRequests(const stateCommon::LoopData& data){
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_MOVE_TAKEN)) {
            _playerToFaceRequests[data.action.payload] = FACE_ELSEWHERE;
        }
        if(data.action.type == GAME_DEF_ACTION_MOVE_REQUEST && timer::runningFor()  == 0) {
            _lastFace = data.face;
            _moveIndex = data.action.payload;
            timer::mark(800, handleViewNormalize);
            bool isInvalid = _playerToFaceRequests[_moveIndex] != FACE_COUNT || _ownership[data.face] != PLAYER_LIMIT;
            if(isInvalid) {
                _viewState = VIEW_STATE_ERROR;
                return;
            }
            _viewState = VIEW_STATE_RADIATE;
            _playerToFaceRequests[_moveIndex] = data.face;
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
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PROGRESS)) {
            changeToProgress();
            return true;
        }
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_END)) {
            timer::cancel();
            stateCommon::handleStateChange(GAME_DEF_STATE_END);
            return true;
        }
        if(buttonMultiClicked()){
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_END, .payload=millis()});
            _isEndInitiator = true;
            timer::cancel();
            stateCommon::handleStateChange(GAME_DEF_STATE_END);
            return false;
        }
        if(buttonDoubleClicked()){
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PROGRESS, .payload = millis()});
            changeToProgress();
            return true;
        }
        return false;
    }

    void handleTransitionToMover() {
        stateCommon::handleStateChange(GAME_DEF_STATE_MOVER);
    }

    void handleMoverTransition(){
        if(isAlone() && timer::runningFor() == 0 && !_isTransitioningToMover){
            _isTransitioningToMover = true;
            timer::mark(100, handleTransitionToMover);
            return;
        }
        if(!isAlone && _isTransitioningToMover == true) {
            timer::cancel();
            _isTransitioningToMover = false;
            return;
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            handleEnter();
        }
        handleMoverTransition();
        updateView();
        processPlayerRequests(data);
        handleProgression(data);
    }

    void reset() {
        FOREACH_FACE(f){
            _ownership[f] = PLAYER_LIMIT;
        }
    }

    byte getRequestsForFace(const byte face, byte* playerBuffer) {
        byte count = 0;
        for(byte i = 0; i < PLAYER_LIMIT; i++) {
            if(_playerToFaceRequests[i] == face){
                playerBuffer[count] = i;
                count++;
            }
        }
        return count;
    }

    void applyOwner(const byte face, const byte playerIndex) {
        _ownership[face] = playerIndex;
    }

    byte getOwnership(const byte face){
        return _ownership[face];
    }

    byte getOffOwnership(const byte face) {
        if(isValueReceivedOnFaceExpired(face)) {
            return PLAYER_LIMIT;
        }
        const byte value = getLastValueReceivedOnFace(face);
        if(value == 0) {
            return PLAYER_LIMIT;
        }
        return value - 1;
    }

    bool isEndInitiator(){
        return _isEndInitiator;
    }
}