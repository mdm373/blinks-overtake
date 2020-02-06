# blinks-overtake 🎮
| My first stab at writing a game for [blinks](https://blinks.games/)

[learn how to play](https://mdm373.github.io/blinks-overtake/)

# developers

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


## Memory Notes

* Empty Sketch: 334 bytes
* Low Memory Warning: 256
* That gives us: 78 bytes to play with
* Global Currently Uses: 281 (68%)