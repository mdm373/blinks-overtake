#include "state-progress.h"
#include "state-board.h"
#include "player.h"
#include "timer.h"
#include "game-def.h"
#include "animate.h"
#include "colors.h"

namespace stateProgress {

    #define VIEW_MOVES_HIGHLIGHT 0
    #define VIEW_MOVES_SHOW 1
    #define VIEW_BLOOM 2

    byte _faceBuffer[FACE_COUNT];
    byte _viewState;
    

    #define CONFLICT_RATE 4
    #define PHASE_DURATION 1000
    
    void handleBloomDone(){
        stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);

    }
    static void drawBloomState() {
        if(_viewState != VIEW_BLOOM) {
            return;
        }
        FOREACH_FACE(f) {
            if(_faceBuffer[f] < PLAYER_LIMIT) {
                stateBoard::applyOwner(f, _faceBuffer[f]);
                animate::animTransitionFace(stateBoard::getFieldColor(f), player::getColor(_faceBuffer[f]), PHASE_DURATION, f);
            }        
        }
    }

    void popBloomFaceBuffer(byte f) {
        byte priorOwner = stateBoard::getOwnership( (FACE_COUNT + f + 1) % FACE_COUNT);
        byte nextOwner = stateBoard::getOwnership( (FACE_COUNT + f - 1) % FACE_COUNT);
        byte offOwner = stateBoard::getOffOwnership(f);
        //Two match, they win!
        if(priorOwner < PLAYER_LIMIT && (priorOwner == nextOwner || priorOwner == offOwner)) {
            _faceBuffer[f] = priorOwner;
            return;
        }
        if(nextOwner < PLAYER_LIMIT && nextOwner == offOwner) {
            _faceBuffer[f] = nextOwner;
            return;
        }

        //one and no others, they win!
        if(priorOwner < PLAYER_LIMIT    && nextOwner >= PLAYER_LIMIT    && offOwner >= PLAYER_LIMIT) {
            _faceBuffer[f] = priorOwner;
            return;
        }
        if(priorOwner >= PLAYER_LIMIT   && nextOwner < PLAYER_LIMIT     && offOwner >= PLAYER_LIMIT) {
            _faceBuffer[f] = nextOwner;
            return;
        }
        if(priorOwner >= PLAYER_LIMIT   && nextOwner >= PLAYER_LIMIT    && offOwner < PLAYER_LIMIT) {
            _faceBuffer[f] = offOwner;
            return;
        }
    }
    static void popBloomFaces(){
        FOREACH_FACE(f){
            _faceBuffer[f] = PLAYER_LIMIT;
            if(stateBoard::getOwnership(f) >= PLAYER_LIMIT) {
                popBloomFaceBuffer(f);
            }
        }
    }

    void handleMovesDone(){
        _viewState = VIEW_BLOOM;
        animate::startAnim();
        popBloomFaces();
        timer::mark(PHASE_DURATION, handleBloomDone);
    }

    void handleMovesHighlightDone(){
        _viewState = VIEW_MOVES_SHOW;
        stateBoard::updateOffOwners();
        animate::startAnim();
        timer::mark(PHASE_DURATION, handleMovesDone);
    }
    
    static void drawMoveState() {
        if(_viewState > VIEW_MOVES_SHOW) {
            return;
        }
        FOREACH_FACE(f) {
            byte count = stateBoard::getRequestsForFace(f, &(_faceBuffer[0]));
            if(count == 0) {
                continue;
            }
            if(count == 1) {
                if(_viewState == VIEW_MOVES_HIGHLIGHT) {
                    animate::animTransitionFace(stateBoard::getFieldColor(f), WHITE, PHASE_DURATION/2, f);
                } else {
                    animate::animTransitionFace(WHITE, player::getColor(_faceBuffer[0]), PHASE_DURATION, f);
                }
                continue;
            }
            byte period = PHASE_DURATION / (count*2);
            byte current = (timer::runningFor() / period) % count;
            setColorOnFace(player::getColor(_faceBuffer[current]), f);
        }
        return;
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data) {
        if(isEnter) {
            _viewState = VIEW_MOVES_HIGHLIGHT;
            animate::startAnim();
            timer::mark(PHASE_DURATION/2, handleMovesHighlightDone);
        }
        stateBoard::drawOwners();
        drawMoveState();
        drawBloomState();
        
    }
}