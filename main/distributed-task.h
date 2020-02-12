
#include "blink-types.h"
#include "state-common.h"

namespace distributedTask {
    typedef byte (taskHandler)(byte op, byte value);
    

    //ing: just before, ed: just after
    #define DISTRIBUTED_TASK_OP_PASSED_IN_NMOD 0
    #define DISTRIBUTED_TASK_OP_PASSING_FORWARD 1
    #define DISTRIBUTED_TASK_OP_PASSED_BACKWARD_NMOD 2
    #define DISTRIBUTED_TASK_OP_PASSING_BACKWARD 3
    #define DISTRIBUTED_TASK_OP_PASSED_DONE_NMOD 4
    #define DISTRIBUTED_TASK_STATE_IDLE 0
    #define DISTRIBUTED_TASK_STATE_PEND 1
    #define DISTRIBUTED_TASK_STATE_DONE 2
    
    void init(const byte requestType, const taskHandler& opHandler, byte value);
    void loop(const stateCommon::LoopData& data, const byte requestType, const taskHandler& opHandler);
    byte getState();
    void reset();
}