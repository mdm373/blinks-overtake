#include "action.h"

namespace action {

    byte prev_type;
    byte prev_payload;

    bool isBroadcastReceived(const Action& incoming, byte type){
        if(incoming.type == type && ( prev_type != incoming.type || prev_payload != incoming.payload)) {
            broadcast(incoming.type,incoming.payload);
            return true;
        }
        return false;
    }
    
    Action decode(const byte* buffer){
        return Action{
            .type=buffer[0],
            .payload=buffer[1]
        };
    }

}