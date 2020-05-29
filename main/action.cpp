#include "action.h"

namespace action {


    bool send(const byte type, const byte payload , const byte face) {

        if(isValueReceivedOnFaceExpired(face)) {
            return false;
        }

        byte packet[ACTION_LEN];
        
        packet[0] = type;
        packet[1] = payload; 

        sendDatagramOnFace( &packet , sizeof(packet), face);

        return true;            

    }

    byte prev_broadcast_type;
    byte prev_broadcast_payload;

    void broadcast(const byte type, const byte payload ) {

        FOREACH_FACE(f) {
            send( type , payload , f );
        }

        prev_broadcast_type    = type;
        prev_broadcast_payload = payload;
        
    }    

    bool isBroadcastReceived(const Action& incoming, const byte type){
        if(incoming.type == type && ( prev_broadcast_type != incoming.type || prev_broadcast_payload != incoming.payload)) {
            broadcast(incoming.type,incoming.payload);
            return true;
        }
        return false;
    }


    bool isBroadcastReceived(const byte incoming_type , const byte incoming_payload, byte type) {
        if( incoming_type == type && ( prev_broadcast_type != incoming_type || prev_broadcast_payload != incoming_payload)) {
            broadcast(incoming_type,incoming_payload);
            return true;
        }
        return false;
    }

    void encode(const Action& action, byte* buffer) {
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
