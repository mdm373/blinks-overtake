#include <ArduinoTypes.h>
#include <blinklib.h>
#include "action.h"

#ifndef HEADER_STATE_COMMON
#define HEADER_STATE_COMMON

namespace stateCommon {
    
    typedef struct LoopData{
        const byte face;
        const action::Action& action;
        LoopData operator=(const LoopData &a) { return a;}
    };

    typedef void (*stateLoop)(LoopData LoopData);
    typedef void (*stateAction)(void);

    #define STATE_COMMON_INDEX_READY 0
    #define STATE_COMMON_INDEX_PLAYER_WIDGET 1
    #define STATE_COMMON_INDEX_PLAYER_INFECT 2
    #define STATE_COMMON_INDEX_PLAYER_ASSIGN 3
    #define STATE_COMMON_INDEX_LENGTH 4
    

    #define STATE_COMMON_ACTION_INVALID 0
    #define STATE_COMMON_ACTION_CHANGE_TO_PLAYER_ASSIGN 2
    #define STATE_COMMON_ACTION_INFECT 3
    #define STATE_COMMON_ACTION_PLAYER_ADDED 4

    void handleStateChange(const byte newState);
    byte getCurrent();
    void addState(const stateLoop loop, const stateAction enter, const stateAction exit, const byte stateIndex);
    void loop(const LoopData& loopData);
    

}

#endif