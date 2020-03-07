#ifndef HEADER_GAME_DEF
    #define HEADER_GAME_DEF

    #include "state-common.h"

    #define GAME_DEF_STATE_PLAYER_ASSIGN 0
    #define GAME_DEF_STATE_MOVER 1
    #define GAME_DEF_STATE_BOARD 2
    #define GAME_DEF_STATE_PROGRESS 3
    #define GAME_DEF_STATE_END 4
    #define GAME_DEF_STATE_LENGTH 5
    #define GAME_DEF_STATE_DEFAULT GAME_DEF_STATE_PLAYER_ASSIGN

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
    #define GAME_DEF_ACTION_PLAYER_INCREMENT 4
    #define GAME_DEF_ACTION_PLAYER_LOCKED 5
    #define GAME_DEF_ACTION_MOVE_REQUEST 6
    #define GAME_DEF_ACTION_MOVE_RESPONSE 7
    #define GAME_DEF_ACTION_MOVE_TAKEN 8
    #define GAME_DEF_ACTION_PROGRESS 9
    #define GAME_DEF_ACTION_END 10
    #define GAME_DEF_ACTION_TOTAL_OWNERSHIP 11 //12 13
    #define GAME_DEF_ACTION_NOOP 14

#endif