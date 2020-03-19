# Blinks: Raid

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

1) **Join at least 4 blinks together and long press any one to form the island.** This step can be done at anytime to start over. The island should now be solid green

2) **Remove a blink from the island's edge.** This will be the player's viking longship. The longship is lit with the current player color on its active edge. The two flanking white faces vaguely make the longship blink resemble a pointy viking helmet. You can remove more longships for other players or share a single longship. To change the longship player color, tap it once.

3) **Everyone take a good long look at the island and decide (in secret) where you want to land your longship.** Once everyone has their minds made up, have all players close their eyes.

4) **Clockwise, one player at a time, open your eyes and make your move by connecting your longship to an unoccupied (green) island edge.** Connect the token using its player color "active" side. The island will blink your color on that face to confirm and then return to green once again. If your all sharing a long ship, give it a tap to move it to the next player color and pass it along. Otherwise, let the next player know its their turn and close your eyes.
  
5) **Once all players have moved, everyone should open their eyes. Double tap blink on the island**. The island fill fade in each players new move over a few seconds showing where that players longship landed this turn. Each player's viking hoard will then spread outwards to occupy neighboring spaces.
    - ✅ **Captured**: Spaces attacked by only a single player token are now occupied by that player. They will pulse the requesting player's color.
    - ❎ **Contested**: Spaces attacked by more than one player are contested. They will alternate blinking each player's color before disappearing. The space will remain unoccupied.

    - ⬅️⬆️➡️ All occupied spaces will then spread their player's armies outwards. To spread, armies capture unoccupied neighbors on the island if a majority of that neighbor's edges are unoccupied or owned by a single player.
    - 🏘️ Spaces on the island are **neighbors** if they are adjacent to one another on the same blink or connected adjacently between two blinks.

6) **Go back to step 3 (with all eyes closed)** if no players admit defeat and there are still unoccupied island edges for players to take moves on.

7) **Otherwise, triple tap the island.** The island will pulse green for a few seconds while it totals up the number of spaces occupied by each player. The winning player (the one with the most spaces) spaces will flash white. In the event of a tie, all spaces will fade to green for each player.

### Additional Play Notes

- Never remove any blink that would break the board apart.
- If, at any point, something goes wrong and a token or the board's colors don't line up, go back to step one.
- If the game detects an illegal attack on the island (attacking an occupied space or two attacks by one player in the same round) it will blink red in response and reject the attack.

## Develop

### Requirements

- [ArduinoIDE](https://www.arduino.cc/en/Guide/HomePage)
- [BlinksSDK](https://github.com/Move38/Blinks-SDK)

### Optionals

- VS Code w/ Microsoft [Arduino plugin](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)