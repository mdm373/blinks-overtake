#ifndef HEADER_STATE_MOVER
#define HEADER_STATE_MOVER

#include "state-common.h"

namespace stateMover {
    void loop(const stateCommon::LoopData& data);
    void enter();
}

#endif