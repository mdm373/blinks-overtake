#include "action.h"
#include <string.h>
#include "blinklib.h"
namespace action {

    void broadcast(const Action& action){
        byte data[ACTION_LEN];
        encode(action, &data[0]);
        FOREACH_FACE(f) {
            if(!isValueReceivedOnFaceExpired(f)) {
                sendDatagramOnFace(&data[0], sizeof(data), f);
                #ifdef DEBUG
                    setColor(WHITE);
                #endif
            }
        }
    }

    void broadcastEmpty(const byte type){
        broadcast(Action{.type = type, .payload=0});
    }


    Action cachedBroadcast{.type=0, .payload=0};
    bool isBroadcastRecieved(const Action& incoming, byte type){
        if(cachedBroadcast.type == incoming.type && cachedBroadcast.payload != incoming.payload) {
            cachedBroadcast = Action{.type=incoming.type, .payload=incoming.payload};
            return true;
        }
        return false;
    }

    void encode(const Action& action, byte* buffer){
        buffer[0] = action.type;
        buffer[1] = action.payload;
    }
    
    Action decode(const byte* buffer){
        return Action{.type=buffer[0], .payload=buffer[1]};
    }

}