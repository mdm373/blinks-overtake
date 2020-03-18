#include "state-player-assign.h"
#include "game-def.h"
#include "action.h"
#include "player.h"
#include "animate.h"
#include "timer.h"
#include "state-board.h"

#pragma clang diagnostic ignored "-Wnarrowing"

namespace statePlayerAssign{
    
    byte _playerCount = 2;
    bool _isShowPlayerCount = false;
    bool _neighboors[FACE_COUNT];
    
    void incrementCount() {
        _playerCount = (_playerCount + 1) % 5;
        if(_playerCount < 2) {
            _playerCount = 2;
        }
    }

    byte getMissingNeighboor(){
        FOREACH_FACE(f) {
            if(_neighboors[f] && isValueReceivedOnFaceExpired(f)){
                return f;
            }
        }
        return FACE_COUNT;
    }

    void handlePlayerLocked(byte nextState) {
        for(int i = 0; i < _playerCount; i++) {
            player::add(i);
        }
        timer::cancel();
        stateBoard::reset();
        stateCommon::handleStateChange(nextState);
    }

    void handleNeighborLost(){
        if(getMissingNeighboor() == FACE_COUNT) {
            return;
        }
        if(!isAlone()){
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_LOCKED, .payload=0});
            handlePlayerLocked(GAME_DEF_STATE_BOARD);
            return;
        }
        handlePlayerLocked(GAME_DEF_STATE_MOVER);        
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            player::reset();
            timer::cancel();
            buttonSingleClicked();
            _playerCount = 2;
            _isShowPlayerCount = startState() == START_STATE_WE_ARE_ROOT;
            FOREACH_FACE(f){
                _neighboors[f] = !isValueReceivedOnFaceExpired(f);
            }
        }
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAYER_LOCKED)){
            handlePlayerLocked(GAME_DEF_STATE_BOARD);
            return;
        }
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAYER_INCREMENT)) {
            _isShowPlayerCount = false;
            incrementCount();
        }
        if(buttonSingleClicked()) {
            _isShowPlayerCount = true;
            incrementCount();
            action::broadcast(action::Action{.type=GAME_DEF_ACTION_PLAYER_INCREMENT, .payload=millis()});
        }
        if(getMissingNeighboor() < FACE_COUNT && timer::runningFor() == 0) {
            timer::mark(700, handleNeighborLost);
        }
        if(!_isShowPlayerCount) {
            setColor(dim(FIELD, 100));
            return;
        }
        FOREACH_FACE(f) {
            if(f > _playerCount-1) {
                setColorOnFace(dim(FIELD, 100), f);
                continue;
            }
            setColorOnFace(player::getColor(f), f);
        }
    }
}