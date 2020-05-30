#include "distributed-task.h"

namespace distributedTask {
    
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

    void sendAllDone(byte requestType, byte taskValue){
        FOREACH_FACE(f){
            action::send( requestType+DONE_TYPE_OFFSET , taskValue , f);
        }
    }
    
    bool respondHandled(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (data.action.type == requestType) {
            action::send( (byte) (requestType+RESP_TYPE_OFFSET) , data.action.payload ,  data.face );
            return true;
        }
        return false;
    }

    void sendBack(const byte requestType, taskHandler& handler, byte taskValue) {
        if(_incomingFace < FACE_COUNT) {
            const bool sent = action::send( (byte)  requestType+RESP_TYPE_OFFSET ,   taskValue,   _incomingFace);
            _state = DISTRIBUTED_TASK_STATE_DONE;
            return;
        }
        sendAllDone(requestType, taskValue);
        reset();
        handler(DISTRIBUTED_TASK_OP_PASSED_DONE, taskValue);
    }

    void sendAroundThenBack(const byte requestType, taskHandler& handler, byte taskValue) {
        bool sent = false;
        while(sent == false && _outgoingFace < FACE_COUNT) {
            if(_outgoingFace == _incomingFace) {
                _outgoingFace = _outgoingFace + 1;
                continue;    
            }
            sent = action::send(  requestType, taskValue ,  _outgoingFace);
            if(!sent){
                _outgoingFace = _outgoingFace + 1;
            }
        }
        if(!sent) {
            sendBack(requestType, handler, taskValue);
            return;
        }
        _state = DISTRIBUTED_TASK_STATE_PEND;
    }
    

    void init(const byte requestType, taskHandler& handler, byte payload) {
        _incomingFace = FACE_COUNT;
        _outgoingFace = 0;
        byte taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_IN, payload);
        sendAroundThenBack(requestType, handler, taskValue);
    }

    void loopIdle(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (data.action.type != requestType) {
            return;
        }
        _incomingFace = data.face;
        _outgoingFace = 0;
        byte taskValue = handler(DISTRIBUTED_TASK_OP_PASSING_IN, data.action.payload);
        sendAroundThenBack(requestType, handler, taskValue);
    }

    void loopPending(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }

        if (data.action.type != (requestType + RESP_TYPE_OFFSET)) {
            return;
        }
        _outgoingFace = _outgoingFace + 1;
        sendAroundThenBack(requestType, handler, data.action.payload);

    }

    void loopDone(const stateCommon::LoopData& data, const byte requestType, taskHandler& handler){
        if (respondHandled(data, requestType, handler)) {
            return;
        }
        
        if (data.action.type == requestType+DONE_TYPE_OFFSET) {
            sendAllDone(requestType, data.action.payload);
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