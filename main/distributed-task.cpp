#include "distributed-task.h"
#include "action.h"
#include "state-common.h"
#include "global-events.h"

namespace distributedTask {
    
    #define NIL_VALUE  0b11111111
    #define RESP_TYPE_OFFSET 1
    #define DONE_TYPE_OFFSET 2
    

    byte _incomingFace = FACE_COUNT;
    byte _outgoingFace = 0;
    byte _state = DISTRIBUTED_TASK_STATE_IDLE;
    
    void reset() {
        _state = DISTRIBUTED_TASK_STATE_IDLE;
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
    }
    
    bool respondHandled(const stateCommon::LoopData& data, const byte requestType, const taskHandler& handler){
        if (data.action.type == requestType) {
            action::send(action::Action{.type = requestType+RESP_TYPE_OFFSET, .payload=NIL_VALUE}, data.face);
            return true;
        }
        return false;
    }

    void sendBack(const byte taskValue, const byte requestType, const taskHandler& handler) {
        if(_incomingFace < FACE_COUNT) {
            const byte modValue = handler(DISTRIBUTED_TASK_OP_PASSING_BACKWARD, taskValue);
            const bool sent = action::send(action::Action{.type = requestType+RESP_TYPE_OFFSET, .payload=modValue}, _incomingFace);
            if(!sent) {
                globalEvents::changeAllToFail();
                return;
            }
            _state = DISTRIBUTED_TASK_STATE_DONE;
            return;
        }
        action::broadcast(action::Action{.type = requestType+DONE_TYPE_OFFSET, .payload= taskValue});
        reset();
        handler(DISTRIBUTED_TASK_OP_PASSED_DONE_NMOD, taskValue);
    }

    void sendAroundThenBack(const byte requestType, const taskHandler& handler, const byte taskValue) {
        bool sent = false;
        while(sent == false && _outgoingFace < FACE_COUNT) {
            if(_outgoingFace == _incomingFace) {
                _outgoingFace = _outgoingFace + 1;
                continue;    
            }
            const byte modValue = handler(DISTRIBUTED_TASK_OP_PASSING_FORWARD, taskValue);
            sent = action::send(action::Action{.type = requestType, .payload=modValue}, _outgoingFace);
            if(!sent){
                _outgoingFace = _outgoingFace + 1;
            }
        }
        if(!sent) {
            sendBack(taskValue, requestType, handler);
            return;
        }
        _state = DISTRIBUTED_TASK_STATE_PEND;
    }
    

    void init(const byte requestType, const taskHandler& handler, byte payload) {
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
        sendAroundThenBack(requestType, handler, payload);
    }

    void loopIdle(const stateCommon::LoopData& data, const byte requestType, const taskHandler& handler){
        if (data.action.type != requestType) {
            return;
        }
        _incomingFace = data.face;
        _outgoingFace = 0;
        handler(DISTRIBUTED_TASK_OP_PASSED_IN_NMOD, data.action.payload);
        sendAroundThenBack(requestType, handler, data.action.payload);
    }

    void loopPending(const stateCommon::LoopData& data, const byte requestType, const taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }

        if (data.action.type != (requestType + RESP_TYPE_OFFSET)) {
            return;
        }
        
        if( data.face != _outgoingFace) {
            globalEvents::changeAllToFail();
            return;
        }
        
        _outgoingFace = _outgoingFace + 1;
        if(data.action.payload != NIL_VALUE) {
            handler(DISTRIBUTED_TASK_OP_PASSED_BACKWARD_NMOD, data.action.payload);
        }
        sendAroundThenBack(requestType, handler, data.action.payload);

    }

    void loopDone(const stateCommon::LoopData& data, const byte requestType, const taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }
        
        if (action::isBroadcastRecieved(data.action, requestType+DONE_TYPE_OFFSET)) {
            reset();
            handler(DISTRIBUTED_TASK_OP_PASSED_DONE_NMOD, data.action.payload);
        }
    }

    void loop(const stateCommon::LoopData& data, const byte requestType, const taskHandler& handler){
        if(_state == DISTRIBUTED_TASK_STATE_IDLE) {
            loopIdle(data, requestType, handler);
            return;
        }
        if(_state == DISTRIBUTED_TASK_STATE_PEND) {
            loopPending(data, requestType, handler);
            return;
        }
        if(_state == DISTRIBUTED_TASK_STATE_DONE) {
            loopDone(data, requestType, handler);
            return;
        }
    }

    byte getState(){
        return _state;
    }
}