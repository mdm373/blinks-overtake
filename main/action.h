#ifndef HEADER_ACTION
    #define HEADER_ACTION
    
    #include "blink-types.h"

    namespace action {

        struct Action {

            const byte type;
            const byte payload;
        };

        #define ACTION_LEN 2

        void broadcast(const byte type, const byte payload );
        bool send(const byte type, const byte payload , const byte face);

        bool isBroadcastReceived(const Action& incoming, const byte type);

        Action decode(const byte* buffer);
        void encode(const Action& action, byte* buffer);
        
        byte inline decode_type( const byte* buffer) {
            return buffer[0];
        }

        byte inline decode_payload( const byte* buffer) {
            return buffer[1];
        }


}

#endif