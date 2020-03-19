# Overtake (Single Widget: Automatic)

> A Turn Based Strategy Game for [blinks](https://blinks.games/)

| Number of Blinks | Number of Players | Duration             | Recommended Ages |
|:----------------:|:-----------------:|:--------------------:|:----------------:|
| 4 - 62           | 2-4               |  10 - 30 minutes     | 12 & Up          |

## Play

<!-- 
## 🎯 [Watch & Learn](https://mdm373.github.io/blinks-overtake/)
-->

### Install

Upload via one of the following methods...

- Build and upload from source (open `./main` in ArduinoIDE)
- Download the latest [hex from releases](https://github.com/mdm373/blinks-overtake/releases) and run included upload script.
  - Assumes avrdude in path or ArduinoIDE installed under `/Applications/Arduino.app/`
  - Assumes Blinks SDK installed under `~/Documents/Arduino/hardware/Move38-Blinks-Library`

### 📝 Step By Step

1) **Join at least 4 blinks together and long press any one to form the board.** This step can be done at anytime to start over. The board should now be dim with one blink indicating the player count.

2) **Tap any blink on the board to change the player count.** The tapped blink will show once face color for each player (Blue, Orange, Pink, Green). Tap away until the correct number of colors are shown, one for each player.

3) **Remove a single blink from the board.** This will be the player move token. The remaining blinks together form the board. The player token will radiate the color of the current player on its active edge. The board should now be mostly dark.

4) **Pass the player token to the first player and all have all players close their eyes.**

5) **Clockwise, one player at a time, open your eyes and make your move by connecting your token to an unoccupied (dark) face.** Connect the token using its radiating "active" side. The board will blink your color on that face to confirm and then go dark once again. The player token should switch to the next player's color.
  
6) **Pass the token to the next player** letting them know they can now move and close your eyes.

7) **Once all players have moved, everyone should open their eyes. Double tap blink on the board**. The board will blink for a few seconds showing the space for each player's new move and then spread player occupied space outwards.
    - ✅ **Overtaken**: Spaces requested by only a single player token will be overtaken. They will pulse the requesting player's color. The space is now occupied by that player.
    - ❎ **Contested**: Spaces requested by more than one player are contested. They will alternate blinking each player's color before disappearing. The space will remain unoccupied.

    - ⬅️⬆️➡️ All occupied spaces will then spread their player color outwards. To spread, spaces overtake their dark neighbors if a majority of that neighbor's edges are unoccupied or owned by one player.
    - 🏘️ Spaces on the board are **neighbors** if they are adjacent to one another on the same blink or connected adjacently between two blinks.

8) **Go back to step 4 (with all eyes closed)** if no players admit defeat and there are still unoccupied board edges for players to take moves on.

9) **Otherwise, double tap the board before any player moves.** The board will spin red for a few seconds while it totals up the number of spaces occupied by each player. It will then flash the color of the winning player (the one with the most spaces). In the event of a tie it will slowly pulse red.

### Additional Play Notes

- Never remove any blink that would break the board apart.
- If, at any point, something goes wrong and a token or the board's colors don't line up, go back to step one.
- If the game detects an illegal move or any other error condition, it will generally blink red, other than when the game is ending, red indicates something has gone (terribly) wrong.

## Develop

### Requirements

- [ArduinoIDE](https://www.arduino.cc/en/Guide/HomePage)
- [BlinksSDK](https://github.com/Move38/Blinks-SDK)

### Optionals

- VS Code w/ Microsoft [Arduino plugin](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)
- g++

`./test/test.cpp` can be used to script scenarios via mock blinklib input. Test scenarios write state and log info to console.

```bash
sh ./scripts/test.sh
```

### Global Memory Notes

- Empty Sketch: 334 bytes
- Low Memory Warning: 256
- That gives us: 78 bytes to play with!
