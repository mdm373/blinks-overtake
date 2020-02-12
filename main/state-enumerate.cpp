#include "state-enumerate.h"
#include "animate.h"
#include "game-def.h"
#include "action.h"
#include "global-events.h"
#include "log.h"
#include "distributed-task.h"

namespace stateEnumerate {
    
    byte _enumeration;
    byte _totalEnumerations;

    byte enumerateHandler(const byte op, const byte payload) {
        if(op == DISTRIBUTED_TASK_OP_PASSED_IN_NMOD) {
            _enumeration = payload;
            _totalEnumerations = payload;
            return;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSING_FORWARD) {
            return _totalEnumerations + 1;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSED_BACKWARD_NMOD) {
            _totalEnumerations = payload;
            return;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSING_BACKWARD) {
            return _totalEnumerations;
        }
        if(op == DISTRIBUTED_TASK_OP_PASSED_DONE_NMOD) {
            _totalEnumerations = payload;
            stateCommon::handleStateChange(GAME_DEF_STATE_PLAYER_ASSIGN);
            return;
        }
        return payload;
    }

    void loop(const stateCommon::LoopData& data){
        distributedTask::loop(data, GAME_DEF_ACTION_ENUMERATE_TASK, enumerateHandler);
        const byte taskState = distributedTask::getState();
        if(taskState == DISTRIBUTED_TASK_STATE_IDLE) {
            animate::pulse(WHITE, 8);
            if(buttonSingleClicked()) {
                distributedTask::init(GAME_DEF_ACTION_ENUMERATE_TASK, enumerateHandler, 0);
            }
        } else if(taskState == DISTRIBUTED_TASK_STATE_PEND) {
            animate::spin(WHITE, 4);
        } else {
            //spinner appears stopped
        }
    }

    void enter(){
        _totalEnumerations = 0;
        _enumeration = 0;
        distributedTask::reset();
    }

    byte getTotalEnumerations(){
        return _totalEnumerations;
    }

    byte getMyEnumeration(){
        return _enumeration;
    }

}