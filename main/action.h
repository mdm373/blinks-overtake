#ifndef HEADER_ACTION
    #define HEADER_ACTION
    
    #include "blink-types.h"

    namespace action {

        struct Action {
            const byte type;
            const byte payload;
        };

        #define ACTION_LEN 2

        void broadcast(const Action& action);
        bool send(const Action& action, const byte face);
        void reset();
        void broadcastEmpty(const byte type);
        bool isBroadcastRecieved(const Action& action, const byte type);
        void encode(const Action& action, byte* buffer);
        Action decode(const byte* buffer);
}

#endif