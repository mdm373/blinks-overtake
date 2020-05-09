#include "state-end.h"
#include "animate.h"
#include "distributed-task.h"
#include "player.h"
#include "state-board.h"
#include "game-def.h"
#include "timer.h"
#include "colors.h"

namespace stateEnd {

    byte _playerIndex = 0;
    byte _playerTotals[PLAYER_LIMIT];
    byte _winner = 0;
    bool _swapFromTo;

    #define MSG_DELAY 100
    #define WIN_PULSE_DURATION 600
    #define DIM_FIELD dim(COLOR_FIELD, 100)

    void handleWinPulse(){
        _swapFromTo = !_swapFromTo;
        animate::startAnim();
        timer::mark(WIN_PULSE_DURATION, handleWinPulse);
    }

    byte totalHandler(const byte op, const byte payload) {
        if(op == DISTRIBUTED_TASK_OP_PASSING_IN) {
            byte count = payload;
            FOREACH_FACE(f){
                if(stateBoard::getOwnership(f) == _playerIndex){
                    count++;
                }
            }
            return count;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSED_DONE) {
            _playerTotals[_playerIndex] = payload;
            _playerIndex++;
            if(_playerIndex < PLAYER_LIMIT && stateBoard::isEndInitiator()){
                timer::mark(MSG_DELAY, _totalInit);
                return payload;
            }
            if(_playerIndex == PLAYER_LIMIT) {
                byte highestCount = 0;
                for(byte i = 0; i < PLAYER_LIMIT; ++i) {
                    if(_playerTotals[i] > highestCount) {
                        highestCount = _playerTotals[i];
                        _winner = i;
                        continue;
                    }
                    if(_playerTotals[i] == highestCount) {
                        _winner = PLAYER_LIMIT;
                    }
                }
                animate::startAnim();
                timer::mark(WIN_PULSE_DURATION, handleWinPulse);
            }
        }
        return payload;
    }
    
    void _totalInit() {
        distributedTask::init(GAME_DEF_ACTION_TOTAL_OWNERSHIP, totalHandler, 0);
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            distributedTask::reset();
            _winner = PLAYER_LIMIT + 1;
            _playerIndex = 0;
            for(int i =0; i > PLAYER_LIMIT; i++){
                _playerTotals[i] = 0;
            }
            if(stateBoard::isEndInitiator()){
                timer::mark(MSG_DELAY, _totalInit);
            }
        }
        distributedTask::loop(data, GAME_DEF_ACTION_TOTAL_OWNERSHIP, totalHandler);
        if(_winner > PLAYER_LIMIT) {
            animate::pulse(COLOR_FIELD, 6);
            return;
        }
        FOREACH_FACE(f) {
            byte owner = stateBoard::getOwnership(f);
            if(owner == PLAYER_LIMIT) {
                setColorOnFace(DIM_FIELD, f);
                continue;
            }
            if(_winner == PLAYER_LIMIT || owner == _winner) {
                Color baseColor = _winner == PLAYER_LIMIT ? DIM_FIELD : WHITE;
                Color from = _swapFromTo ? baseColor : player::getColor(owner);
                Color to = _swapFromTo ? player::getColor(owner) : baseColor;
                animate::animTransitionFace(from, to, WIN_PULSE_DURATION, f);
                continue;
            }
            setColorOnFace(dim(player::getColor(owner), 150), f);
        }
    }
}