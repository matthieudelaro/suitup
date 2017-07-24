#include "utils.h"

CRGB ALL[NUM_LEDS_ALL];

byte incomingByte = 0; // for incoming Serial1 data
unsigned long previousMillis = 0;
boolean FromLoop = 0;
byte current = 97;

byte testee() {
    return incomingByte;
}
