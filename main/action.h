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

        bool isBroadcastReceived(const Action& action, const byte type);
        Action decode(const byte* buffer);

        void inline encode(const Action& action, byte* buffer) {
            buffer[0] = action.type;
            buffer[1] = action.payload;
        }

}

#endif