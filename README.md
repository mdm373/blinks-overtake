# Overtake

> A Turn Based Strategy Game for [blinks](https://blinks.games/)

| Number of Blinks | Number of Players | Duration             | Recommended Ages |
|:----------------:|:-----------------:|:--------------------:|:----------------:|
| 4 - 62           | 2-4               |  10 - 30 minutes     | 12 & Up          |

## Play

<!-- 
## 🎯 [Watch & Learn](https://mdm373.github.io/blinks-overtake/)
-->

### Install

- Build and upload from source (open `./main` in Arduiono IDE)
- Download the latest [hex from releases](https://github.com/mdm373/blinks-overtake/releases) and run included upload script.
  - Assumes avrdude in path or ArduinoIDE installed under `/Applications/Arduino.app/`
  - Assumes Blinks SDK installed under `~/Documents/Arduino/hardware/Move38-Blinks-Library`

### 📝 Step By Step

1) **Join at least 4 blinks together and double tap any one to form the board.** This step can be done at anytime to start over. The board should now be slowly pulsing white.

2) **Single tap any blink on the board.** The board will spin white for a few seconds while it sets up before switching to the first player's color (orange)

3) **Remove a single blink from the board.** This is the player's token and they should hang onto it. Wait for the board to change to the next player color before the next token is removed. The removed player token will radiate the its player color on its "active" side. Repeat this step for each player.

4) **Click the board and all have all players close their eyes.** The board should now be dark. The board will automatically go dark after 4 players have taken their tokens or if it goes down to 2 blinks. No clicking required.

5) **Clockwise, one player at a time, open your eyes and make your move by connecting your token to an unoccupied (dark) face on the board edge.** The board will blink your color on that face to confirm and then go dark once again. Announce to the next player that they can move and close your eyes.

6) **Once all players have moved, everyone should open their eyes. Tap a single blink on the board**.
    - The board will pulse for two seconds showing each players move, players who chose the same space will alternate blinking each player's color and turn off.

    - All occupied spaces will then spread outwards with their player color and occupy any dark neighbor if a majority of that neighbors edges are unoccupied or owned by the player.

7) **Go back to step 5** if no players admit defeat and there are still unoccupied board edges for players to capture.

8) **Otherwise, tap the board once before any player moves.** The board will spin red for a few seconds while it totals up the number of spaces occupied by each player.The board will then flash the color of the winning player (the one with the most spaces). In the event of a tie it will slowly pulse red.

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
