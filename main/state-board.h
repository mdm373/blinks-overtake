
#ifndef HEADER_STATE_BOARD
    #define HEADER_STATE_BOARD

    #include "state-common.h"

    namespace stateBoard {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
        void reset();
        void drawOwners();
        byte getRequestsForFace(const byte face, byte* playerBuffer);
        void applyOwner(const byte face, const byte playerIndex);
        byte getOwnership(const byte face);
        byte getOffOwnership(const byte face);
        void updateOffOwners();
        bool isEndInitiator();
        Color getFieldColor(const byte face);
    }
    
#endif