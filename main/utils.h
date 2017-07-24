#ifndef UTILS_1397483
#define UTILS_1397483

#include "FastLED.h"

#define NUM_LEDS_ARM_L 24
#define NUM_LEDS_ARM_R 24
#define NUM_LEDS_CHEST 176
#define NUM_LEDS_LEG_L 60
#define NUM_LEDS_LEG_R 60
#define NUM_LEDS_SKIRT 200
//#define NUM_LEDS_ALL NUM_LEDS_ARM_L+NUM_LEDS_ARM_R+NUM_LEDS_CHEST+NUM_LEDS_LEG_L+NUM_LEDS_LEG_R
#define NUM_LEDS_ALL NUM_LEDS_ARM_L+NUM_LEDS_ARM_R+NUM_LEDS_CHEST+NUM_LEDS_SKIRT

extern CRGB ALL[NUM_LEDS_ALL];

extern byte incomingByte; // for incoming Serial1 data
extern unsigned long previousMillis;
extern boolean FromLoop;
extern byte current;


#endif
