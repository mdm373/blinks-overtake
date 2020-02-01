#include <ArduinoTypes.h>

#ifndef HEADER_ACTION
#define HEADER_ACTION

namespace action {

    typedef struct Action {
        const byte type;
        const byte payload;
        Action operator=(const Action &a) { return a;}
    };

    #define ACTION_LEN 3

    void broadcast(const Action& action);
    void broadcastEmpty(const byte type);
    bool isBroadcastRecieved(const Action& action);
    void encode(const Action& action, byte* buffer);
    Action decode(const byte* buffer);
}

#endif