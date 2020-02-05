#ifndef HEADER_GAME_DEF
#define HEADER_GAME_DEF

#include "state-common.h"

namespace gameDef {
    #define GAME_DEF_STATE_FAIL 0
    #define GAME_DEF_STATE_ENUM_NONE 1
    #define GAME_DEF_STATE_ENUM_PEND 2
    #define GAME_DEF_STATE_ENUM_ASGN 3
    #define GAME_DEF_STATE_PLAYER_ASSIGN 4
    #define GAME_DEF_STATE_MOVER 5
    #define GAME_DEF_STATE_BOARD 6
    #define GAME_DEF_STATE_LENGTH 7

    #if GAME_DEF_STATE_LENGTH != STATE_COMMON_STATE_LEN
        #error "game state length must equal state machine state length" //to state dep cycle, make len requirement explicit
    #endif

    #define GAME_DEF_ACTION_INVALID 0
    #define GAME_DEF_ACTION_RESET 1
    #define GAME_DEF_ACTION_FAIL 2
    #define GAME_DEF_ATION_STATE_CHG 3
    #define GAME_DEF_ACTION_ENUMERATE_REQUEST 4
    #define GAME_DEF_ACTION_ENUMERATE_RESPONSE 5
    #define GAME_DEF_ACTION_TO_STATE_PLAYER_ASSIGN 6
    #define GAME_DEF_ACTION_PLAYER_PICKED 7
    #define GAME_DEF_ACTION_START 8
    #define GAME_DEF_ACTION_MOVE_REQUEST 9
    #define GAME_DEF_STATE_DEFAULT GAME_DEF_STATE_ENUM_NONE

}

#endif