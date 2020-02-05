#include <iostream>

#include "blinklib-mock.h"
#include "../src/game-def.h"
#include "../src/action.h"
#include "../src/main.h"

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
    sendAction(0, action::Action{.type= GAME_DEF_ACTION_ENUMERATE_REQUEST, .payload=1});
    loop();
    blinklibMock::expireValuesOnAllFacesExceptBoth(0, 4);
    sendAction(4, action::Action{.type= GAME_DEF_ACTION_ENUMERATE_RESPONSE, .payload=2});
    loop();
    blinklibMock::expireValuesOnAllFacesExceptBoth(0, 4);
    sendAction(0, action::Action{.type=GAME_DEF_ACTION_TO_STATE_PLAYER_ASSIGN, .payload=6});
    loop();
}