#include "state-fail.h"
#include "animate.h"

namespace stateFail {
    void loop(const stateCommon::LoopData& data){
        animate::pulse(RED, 4);
    }
}