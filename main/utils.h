#ifndef UTILS_1397483
#define UTILS_1397483

#define byte unsigned char

#define NUM_LEDS_ARM_L 24 // LEFT, seen from the guy wearing it
#define NUM_LEDS_ARM_R 24
#define NUM_LEDS_CHEST 176
#define NUM_LEDS_LEG_L 60
#define NUM_LEDS_LEG_R 60
#define NUM_LEDS_SKIRT 200
// #define NUM_LEDS_ALL (NUM_LEDS_ARM_L+NUM_LEDS_ARM_R+NUM_LEDS_CHEST+NUM_LEDS_SKIRT)

extern byte incomingByte; // for incoming Serial1 data
extern unsigned long previousMillis;
extern bool FromLoop;
extern byte current;

#define LENGTH_ARM 8
#define FAT_ARM (NUM_LEDS_ARM_R/LENGTH_ARM)
#define WIDTH_CHEST 16
#define WIDTH (LENGTH_ARM + WIDTH_CHEST + LENGTH_ARM)

#define SUIT_FOR_MEN 1
#ifdef SUIT_FOR_MEN
    #define NUM_LEDS_ALL (NUM_LEDS_ARM_L+NUM_LEDS_ARM_R+NUM_LEDS_CHEST+NUM_LEDS_LEG_L+NUM_LEDS_LEG_R)
    #define HEIGHT_CHEST 14
    #define WIDTH_LEG 5
    #define HEIGHT_LEG 12
    #define HEIGHT (HEIGHT_CHEST + HEIGHT_LEG)
#endif


#ifdef SERIAL_USB
  #define SERIAL Serial
#else
    // #ifdef SERIAL_RADIO
      #define SERIAL Serial1
    // #endif
#endif
#ifdef DEBUG // define macros to print on SERIAL in debug mode..
    #define DSERIAL(arg) SERIAL.print(arg)
    #define DSERIALln(arg) SERIAL.println(arg)
#else // and to do nothing in release mode
    #define DSERIAL(arg)
    #define DSERIALln(arg)
#endif

// #define B(x, minInclude, max) if (x >= 18) \
                    // printf("Vous etes majeur %s\n", nom);

#define B(minInclude, x, maxExclude) (minInclude <= x && x < maxExclude)
#define MSM(minutes, seconds, milliseconds) ((long)((60 * minutes + seconds) * 1000 + milliseconds))
// #define WAIT(duration)  if (duration > 0) \
//                             WasteTime(duration); \
//                         else \
//                             if (Serial1.available() > 0) \
//                             { \
//                               incomingByte = Serial1.read(); \
//                               Serial1.print("I received (DEC): "); \
//                               Serial1.println(incomingByte); \
//                               current = incomingByte; \
//                               FromLoop = 0; \
//                               FastLED.clear(true); \
//                             } \


// Return the index of the LED (in the array ALL)
// as if it was a 2D-array, with (0,0) being in the top-left corner
// (seen from the audience) with the arms horizontal.
// In case of a location (x,y) which does not exist,
// the function returns -1
short id(signed char x, signed char y);


#endif
