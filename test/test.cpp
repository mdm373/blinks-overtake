#include <iostream>

#include "blinklib-mock.h"
#include "../main/game-def.h"
#include "../main/action.h"
#include "../main/main.h"

void sendAction(const byte face, const action::Action& action) {
    action::encode(action, &blinklibMock::datagramPostBuffer[0]);
    blinklibMock::postDatagramOnFace(face, ACTION_LEN);
}

void loop() {
    std::cout << "--looping" << std::endl;
    _main_::loop();
    blinklibMock::resetMocks();
}

int main()  {
    std::cout << "--setting up" << std::endl;
    _main_::setup();
    loop();
    blinklibMock::expireValuesOnAllFacesExceptBoth(0, 4);
    // sendAction(0, action::Action{.type= GAME_DEF_ACTION_ENUMERATE_REQUEST, .payload=1});
    loop();
    blinklibMock::expireValuesOnAllFacesExceptBoth(0, 4);
    // sendAction(4, action::Action{.type= GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=2});
    loop();
    blinklibMock::expireValuesOnAllFacesExceptBoth(0, 4);
    sendAction(0, action::Action{.type=GAME_DEF_ACTION_ASSGN_PLAYERS, .payload=6});
    loop();
}