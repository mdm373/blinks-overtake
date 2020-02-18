#include "state-end.h"
#include "animate.h"
#include "distributed-task.h"
#include "player.h"
#include "state-board.h"
#include "game-def.h"
#include "timer.h"

namespace stateEnd {

    byte _playerIndex = 0;
    byte _playerTotals[PLAYER_LIMIT];
    byte _winner = 0;
    
    #define MSG_DELAY 300
    byte totalHandler(const byte op, const byte payload) {
        if(op == DISTRIBUTED_TASK_OP_PASSING_IN) {
            byte count = payload;
            FOREACH_FACE(f){
                if(stateBoard::getOwnershipe(f) == _playerIndex){
                    count++;
                }
            }
            return count;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSED_DONE) {
            _playerTotals[_playerIndex] = payload;
            _playerIndex++;
            action::reset();
            if(_playerIndex < player::getCount() && stateBoard::isEndInitator()){
                timer::mark(MSG_DELAY, _totalInit);
                return payload;
            }
            if(_playerIndex == player::getCount()) {
                byte highestCount = 0;
                for(byte i = 0; i < player::getCount(); ++i) {
                    if(_playerTotals[i] > highestCount) {
                        highestCount = _playerTotals[i];
                        _winner = i;
                        continue;
                    }
                    if(_playerTotals[i] == highestCount) {
                        _winner = PLAYER_LIMIT;
                    }
                }
            }
        }
        return payload;
    }
    
    void _totalInit() {
        distributedTask::init(GAME_DEF_ACTION_TOTAL_OWNERSHIPE, totalHandler, 0);
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            _winner = PLAYER_LIMIT + 1;
            _playerIndex = 0;
            if(stateBoard::isEndInitator()){
                timer::mark(MSG_DELAY, _totalInit);
            }
        }
        distributedTask::loop(data, GAME_DEF_ACTION_TOTAL_OWNERSHIPE, totalHandler);
        if(_winner > PLAYER_LIMIT) {
            animate::spin(RED, 4);
            return;
        }
        if(_winner == PLAYER_LIMIT) {
            animate::pulse(RED, 16);
            return;
        }
        animate::pulse(player::getColor(_winner), 2);
    }
}