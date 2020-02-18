#include "distributed-task.h"
#include "action.h"
#include "state-common.h"
#include "global-events.h"

namespace distributedTask {
    
    #define NIL_VALUE  0b11111100
    #define RESP_TYPE_OFFSET 1
    #define DONE_TYPE_OFFSET 2
    

    byte _incomingFace = FACE_COUNT;
    byte _outgoingFace = 0;
    byte _state = DISTRIBUTED_TASK_STATE_IDLE;
    byte _taskValue;
    
    void reset() {
        _taskValue = 0;
        _state = DISTRIBUTED_TASK_STATE_IDLE;
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
    }
    
    bool respondHandled(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (data.action.type == requestType) {
            action::send(action::Action{.type = static_cast<byte>(requestType+RESP_TYPE_OFFSET), .payload=NIL_VALUE}, data.face);
            return true;
        }
        return false;
    }

    void sendBack(const byte requestType, taskHandler& handler) {
        if(_incomingFace < FACE_COUNT) {
            _taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_BACKWARD, _taskValue);
            const bool sent = action::send(action::Action{.type = static_cast<byte>(requestType+RESP_TYPE_OFFSET), .payload=_taskValue}, _incomingFace);
            _state = DISTRIBUTED_TASK_STATE_DONE;
            return;
        }
        action::broadcast(action::Action{.type = static_cast<byte>(requestType+DONE_TYPE_OFFSET), .payload= _taskValue});
        byte value = _taskValue;
        reset();
        handler(DISTRIBUTED_TASK_OP_PASSED_DONE, value);
    }

    void sendAroundThenBack(const byte requestType, taskHandler& handler) {
        bool sent = false;
        while(sent == false && _outgoingFace < FACE_COUNT) {
            if(_outgoingFace == _incomingFace) {
                _outgoingFace = _outgoingFace + 1;
                continue;    
            }
            _taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_FORWARD, _taskValue);
            sent = action::send(action::Action{.type = requestType, .payload=_taskValue}, _outgoingFace);
            if(!sent){
                _outgoingFace = _outgoingFace + 1;
            }
        }
        if(!sent) {
            sendBack(requestType, handler);
            return;
        }
        _state = DISTRIBUTED_TASK_STATE_PEND;
    }
    

    void init(const byte requestType, taskHandler& handler, byte payload) {
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
        _taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_IN, payload);
        sendAroundThenBack(requestType, handler);
    }

    void loopIdle(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (data.action.type != requestType) {
            return;
        }
        _incomingFace = data.face;
        _outgoingFace = 0;
        _taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_IN, data.action.payload);
        sendAroundThenBack(requestType, handler);
    }

    void loopPending(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }

        if (data.action.type != (requestType + RESP_TYPE_OFFSET)) {
            return;
        }
        _outgoingFace = _outgoingFace + 1;
        if(data.action.payload != NIL_VALUE) {
            _taskValue = handler(DISTRIBUTED_TASK_OP_PASSED_BACKWARD, data.action.payload);
        }
        sendAroundThenBack(requestType, handler);

    }

    void loopDone(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }
        
        if (action::isBroadcastRecieved(data.action, requestType+DONE_TYPE_OFFSET)) {
            reset();
            handler(DISTRIBUTED_TASK_OP_PASSED_DONE, data.action.payload);
        }
    }

    void loop(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
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