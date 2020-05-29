#include "action.h"

namespace action {


    bool inline send(const byte type, const byte payload , const byte face) {

        if(isValueReceivedOnFaceExpired(face)) {
            return false;
        }

        uint16_t packet = type | ( ( (uint16_t) payload ) << 8 ); 

        sendDatagramOnFace( &packet , sizeof(packet), face);

        return true;            

    }

    byte prev_broadcast_type;
    byte prev_broadcast_payload;

    inline void broadcast(const byte type, const byte payload ) {

        FOREACH_FACE(f) {
            send( type , payload , f );
        }

        prev_broadcast_type    = type;
        prev_broadcast_payload = payload;
        
    }    

    bool isBroadcastReceived(const Action& incoming, byte type){
        if(incoming.type == type && ( prev_broadcast_type != incoming.type || prev_broadcast_payload != incoming.payload)) {
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
