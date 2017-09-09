# Suit up!
Suitup is an Arduino project, designed to control two dance LED costumes.

The final representation can be seen on YouTube: you may watch it from [the beginning of the dance](https://youtu.be/k2F2FrwSzXY?t=17m3s) or from [the first usage of the LEDs](https://youtu.be/k2F2FrwSzXY?t=18m17s)
Big thanks to those who created the costumes and the video player. Big thanks to the dancers, the team in charge of the stage and of the control room for this awesome representation in front of ~1300 people.

(The contrast of the video is pretty bad, so here is what's going on: a video in the background displays light dots moving on the huge screen. Two dancers perform a dance in front of the screen, as if they were actually controlling the moving dots. Eventually the costumes turn on and perform animations to enhance the visual result).

## How to compile ?
The best way is to compile using Arduino IDE (select "Arduino Micro" target). Check and update C macros from [utils.h](https://github.com/matthieudelaro/suitup/blob/master/main/utils.h#L4-L14) to select your target (compile for the man, compile for the man, compile to consider serial communication over USB or over radio, (de)activate debug logs, etc)

You may also run unit tests on some functions with `make test` (requires googletest, cf [Makefile](https://github.com/matthieudelaro/suitup/blob/master/main/Makefile)), and generate CSV files with `make csv` (further explainations below).

## How does it work ?
Because of synchronization and flexibility concerns (with the rest of the representation), the costumes are remotly controlled: a computer plays the video (which we can see behing the dancers), and sends radio signals to the costumes whenever they should start an animation. The costumes play the animation, and wait for another signal.

The signal is composed of two characters: [M|F|A][a letter]. The first character describes de target (M: Male / F: Female / A: All). The second character describes the animation to be performed.

The custom video player implemented for this representation (based on VLC) reads a CSV file (similar to a file describing subtitles). It contains timecodes and associated text, which is then emitted by radio as the player plays the video.

The arduino displays animation by computing the color of each LED. Those are adressed thanks to the array [LEDs](https://github.com/matthieudelaro/suitup/blob/master/main/main.ino#L33), each cell corresponding to the color of a LED. [This video](https://github.com/matthieudelaro/suitup/raw/master/LEDorder_LQ.mp4) shows LEDs being turn on, one after another, from LEDs[0] to LEDs[about 400]. 

