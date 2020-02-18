# blinks-overtake 🎮
> My first stab at writing a game for [blinks](https://blinks.games/)

# Play 

## 🎯 [Watch & Learn](https://mdm373.github.io/blinks-overtake/)

## 📝 Step By Step

1) Join all blinks together and double tap any one to form the board. This step can be done at anytime to start over. The board should now be slowly pulsing white.

2) Single tap a a blink on the board. The board will spin white for a few seconds while it sets up before switching to the first player's color (orange)

3) One player at a time, remove a single blink from the board. This is the player's token and they should hang onto it. Wait for the board to change to the next player color before the next token is removed. 

> Do not remove any blink that would break the board apart

> The board's color will rotate to the next player color after each token is removed (Blue then Orange, Magenta and finally Green)

> The removed player token will radiate the its player color on its "active" side

> There must be at least two blinks on the board

> If at any point while picking players something goes wrong and a token or the board's colors dont line up, go back to step one and

4) Once four players have taken their tokens or no more tokens can be removed the board will turn dark. Decide on a player order, and have all players close their eyes.

5) One at a time each player should connect their token to an unoccupied / dark face on the board edge. The board will blink your color on that edge space to confirm your move and then go dark once again. Announce to the next player that they can move and close your eyes.

6) Once all players have moved, everyone open your eyes and tap one blink on the board to reveal each players move

| The board will pulse for two seconds showing each players move, players who chose the same space will alternate blinking one anothers colors and turn off

| All player spaces will then spread outwards and consume any dark neighbor assuming a majority of that neighbors edges are unoccupied or owned by the player.

7) If no players admit defeat and there are still unoccupied board edges, go back to step 5. Othwerwise, tap the board before any player moves.

8) The board will spin red for a few seconds while it totals up each players space and then flash the winning player (the one with the most spaces) color. In the event of a tie it will slowly pulse red.




# Develop

## Requirements
* [ArduinoIDE](https://www.arduino.cc/en/Guide/HomePage)
* [BlinksSDK](https://github.com/Move38/Blinks-SDK)

## Optionals
* VS Code w/ Microsoft [Arduino plugin](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)


## Game Instructions


## Mock Debug

### Additional Requirements
* g++

`./test/test.cpp` can be used to script scenarios via mock blinklib input. Test scenarios write state and log info to console.
```
sh ./scripts/test.sh
```


## Global Memory Notes

* Empty Sketch: 334 bytes
* Low Memory Warning: 256
* That gives us: 78 bytes to play with
* Global Currently Uses: 281 (68%)