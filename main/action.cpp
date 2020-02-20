#include "action.h"

namespace action {

    byte cachedBroadcast[ACTION_LEN];
    void broadcast(const Action& action){
        encode(action, &cachedBroadcast[0]);
        FOREACH_FACE(f){
            send(action, f);
        }
    }

    void reset(){
        encode(Action{.type=0, .payload=0}, &cachedBroadcast[0]);
    }

    bool send(const Action& action, const byte face){
        if(isValueReceivedOnFaceExpired(face)) {
            return false;
        }
        byte data[ACTION_LEN];
        encode(action, &data[0]);
        sendDatagramOnFace(&data[0], sizeof(data), face);
        return true;
    }

    void broadcastEmpty(const byte type){
        broadcast(Action{.type = type, .payload=0});
    }

    bool isBroadcastRecieved(const Action& incoming, byte type){
        const Action cached = decode(&cachedBroadcast[0]);
        if(incoming.type == type && (cached.type != incoming.type || cached.payload != incoming.payload)) {
            broadcast(incoming);
            return true;
        }
        return false;
    }

    void encode(const Action& action, byte* buffer){
        buffer[0] = action.type;
        buffer[1] = action.payload;
    }
    
    Action decode(const byte* buffer){
        return Action{
            .type=buffer[0],
            .payload=buffer[1]
        };
    }

}