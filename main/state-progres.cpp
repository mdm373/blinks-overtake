#include "state-progress.h"
#include "state-board.h"
#include "player.h"
#include "timestamp.h"
#include "game-def.h"
#include "animate.h"

namespace stateProgress {
    byte _faceBuffer[FACE_COUNT];
    bool _drawMoves;
    bool _drawBloom;
    bool _sidesReset;

    #define CONFLICT_RATE 4
    #define PHASE_DURATION 2000
    
    static bool handleBloom() {
        if(!_drawBloom) {
            return false;
        }
        const DOUBLE_BYTE duration = timestamp::getDuration();
        if(duration > PHASE_DURATION) {
            _drawBloom = false;
            timestamp::clear();
            stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
            return true;
        }
        FOREACH_FACE(f) {
            const byte owner = stateBoard::getOwnershipe(f);
            if(owner == PLAYER_LIMIT) {
                continue;
            }
            if(_faceBuffer[f] != PLAYER_LIMIT) {
                animate::pulseFace(f, player::getColor(_faceBuffer[f]), 4);
                continue;
            }        
        }
        return true;
    }

    static void popBloomFaces(){
        FOREACH_FACE(f){
            if(stateBoard::getOwnershipe(f) == PLAYER_LIMIT) {
                byte priorOwner = stateBoard::getOwnershipe( (FACE_COUNT + f + 1) % FACE_COUNT);
                byte nextOwner = stateBoard::getOwnershipe( (FACE_COUNT + f - 1) % FACE_COUNT);
                byte offOwner = stateBoard::getOffOwnershipe(f);

                //Two match, they win!
                if(priorOwner != PLAYER_LIMIT && (priorOwner == nextOwner || priorOwner == offOwner)) {
                    _faceBuffer[f] = priorOwner;
                    continue;
                }
                if(nextOwner != PLAYER_LIMIT && nextOwner == offOwner) {
                    _faceBuffer[f] = nextOwner;
                    continue;
                }

                //one and no others, they win!
                if(priorOwner != PLAYER_LIMIT && nextOwner == PLAYER_LIMIT && offOwner == PLAYER_LIMIT) {
                    _faceBuffer[f] = priorOwner;
                    continue;
                }
                if(priorOwner == PLAYER_LIMIT && nextOwner != PLAYER_LIMIT && offOwner == PLAYER_LIMIT) {
                    _faceBuffer[f] = nextOwner;
                    continue;
                }
                if(priorOwner == PLAYER_LIMIT && nextOwner == PLAYER_LIMIT && offOwner != PLAYER_LIMIT) {
                    _faceBuffer[f] = offOwner;
                    continue;
                }
            }
            _faceBuffer[f] = PLAYER_LIMIT;
        }
        FOREACH_FACE(f) {
            if(_faceBuffer[f] != PLAYER_LIMIT) {
                stateBoard::applyOwner(f, _faceBuffer[f]);
                
            }
        }
    }
    static bool handleMoves() {
        if(!_drawMoves) {
            return false;
        }
        const DOUBLE_BYTE duration = timestamp::getDuration();
        if(!_sidesReset && duration > (PHASE_DURATION/2)){
            _sidesReset = true;
            stateBoard::updateOffOwners();
        }
        if(duration > PHASE_DURATION) {
            _drawMoves = false;
            _drawBloom = true;
            popBloomFaces();
            timestamp::mark();
            return false;
        }

        FOREACH_FACE(f) {
            byte count = stateBoard::getRequestsForFace(f, &_faceBuffer[0]);
            if(count == 0) {
                continue;
            }
            if(count == 1) {
                animate::pulseFace(f, player::getColor(_faceBuffer[0]), 4);
                continue;
            }
            byte period = PHASE_DURATION / (count*2);
            byte current = (duration / period) % count;
            setColorOnFace(player::getColor(_faceBuffer[current]), f);
            
        }
        return true;
    }

    void loop(const stateCommon::LoopData& data) {
        stateBoard::drawOwners();
        if(handleMoves()) {
            return;
        }
        handleBloom();
        
    }
    void enter() {
        _drawMoves = true;
        _sidesReset = false;
        _drawBloom = false;
        timestamp::mark();
    }
}