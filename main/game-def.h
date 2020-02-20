#ifndef HEADER_GAME_DEF
    #define HEADER_GAME_DEF

    #include "state-common.h"

    #define GAME_DEF_STATE_ENUM_NONE 0
    #define GAME_DEF_STATE_PLAYER_ASSIGN 1
    #define GAME_DEF_STATE_MOVER 2
    #define GAME_DEF_STATE_BOARD 3
    #define GAME_DEF_STATE_PROGRESS 4
    #define GAME_DEF_STATE_END 5
    #define GAME_DEF_STATE_LENGTH 6
    #define GAME_DEF_STATE_DEFAULT GAME_DEF_STATE_ENUM_NONE

    /*
    * Avoiding a dependency cycle between this game definition and the more generalized state machine.
    * Keeping the heirarchy GameDefintions -> StateMachine w/out needing added indirection level
    * Explicitly enforce state machine size matches game state count at build time
    */
    #if GAME_DEF_STATE_LENGTH != STATE_COMMON_STATE_LEN
        #error "game state length must equal state machine state length"
    #endif

    #define GAME_DEF_ACTION_INVALID 0
    #define GAME_DEF_ACTION_RESET 1
    #define GAME_DEF_ACTION_STATE_CHG 2
    #define GAME_DEF_ACTION_ENUMERATE_TASK 3 //4 5
    #define GAME_DEF_ACTION_ASSIGN_PLAYERS 6
    #define GAME_DEF_ACTION_PLAYER_PICKED 7
    #define GAME_DEF_ACTION_START 8
    #define GAME_DEF_ACTION_MOVE_REQUEST 9
    #define GAME_DEF_ACTION_MOVE_TAKEN 10
    #define GAME_DEF_ACTION_PROGRESS 11
    #define GAME_DEF_ACTION_END 12
    #define GAME_DEF_ACTION_TOTAL_OWNERSHIP 13 //14 15
    #define GAME_DEF_ACTION_NOOP 16

#endif