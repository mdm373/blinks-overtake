
#ifndef HEADER_STATE_BOARD
    #define HEADER_STATE_BOARD

    #include "state-common.h"

    namespace stateBoard {
        void loop(const stateCommon::LoopData& data);
        void enter();
        void reset();
        void drawOwners();
        byte getRequestsForFace(const byte face, byte* playerBuffer);
        void applyOwner(const byte face, const byte playerIndex);
        byte getOwnershipe(const byte face);
        byte getOffOwnershipe(const byte face);
        void updateOffOwners();
    }
    
#endif