#include "FastLED.h"

#define SERIAL_USB 1 // See SERIAL in utils.h
// #define SERIAL_RADIO 1 // See SERIAL in utils.h
#define DEBUG 1 // See DSERIAL and DSERIALln in utils.h
#define ONLY_REMOTE_SYNC 1

#include "utils.h"

// const CRGB purple = CRGB(222, 0, 228); // inspired from original value: CRGB(222, 69, 228)
// const CRGB yellow = CRGB(232, 246, 0); // inspired from original value: CRGB(232, 246, 93)
// const CRGB cyan  = CRGB(0, 250, 243);  // inspired from original value: CRGB(105, 250, 243)
// const CRGB orange  = CRGB(185, 118, 0);  // inspired from original value: CRGB(185, 118, 53)

const CRGB purple = CRGB(128, 0, 115); // inspired from original value: CRGB(222, 69, 228)
const CRGB yellow = CRGB(115, 123, 0); // inspired from original value: CRGB(232, 246, 93)
const CRGB cyan  = CRGB(0, 128, 120);  // inspired from original value: CRGB(105, 250, 243)
const CRGB orange  = CRGB(92, 59, 0);  // inspired from original value: CRGB(185, 118, 53)

// const CRGB purple = CRGB(222, 69, 228); // the original value
// const CRGB yellow = CRGB(232, 246, 93); // the original value
// const CRGB cyan  = CRGB(105, 250, 243);  // the original value
// const CRGB orange  = CRGB(185, 118, 53);  // the original value


CRGB LEDs[NUM_LEDS_ALL];

void setup() {
  byte x = incomingByte;
  FastLED.addLeds<NEOPIXEL, 4>(LEDs, 0, NUM_LEDS_ARM_L);
  FastLED.addLeds<NEOPIXEL, 3>(LEDs, NUM_LEDS_ARM_L+NUM_LEDS_CHEST, NUM_LEDS_ARM_R);
  FastLED.addLeds<NEOPIXEL, 2>(LEDs, NUM_LEDS_ARM_L, NUM_LEDS_CHEST);

  #ifdef SUIT_FOR_MAN
    //For Trouser
    FastLED.addLeds<NEOPIXEL, 6>(LEDs, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R, NUM_LEDS_LEG_L);
    FastLED.addLeds<NEOPIXEL, 5>(LEDs, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R+NUM_LEDS_LEG_L, NUM_LEDS_LEG_R);
  #else
    //For Skirt
    FastLED.addLeds<NEOPIXEL, 5>(LEDs, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R, NUM_LEDS_SKIRT);
  #endif

  SERIAL.begin(9600);
  current = '!';
}

void loop() {
  FastLED.clear(true);
  FromLoop = 1;
  if (SERIAL.available() > 0) {
                // read the incoming byte:
    incomingByte = SERIAL.read();

                // say what you got:
    DSERIAL("I received (DEC): ");
    DSERIALln(incomingByte);
    current = incomingByte;
    FastLED.clear(true);
  }

  if (current == '!') {
    // do nothing
  }
  // else if (current == 97){defile(80);}
  // else if (current == 122){rvbWheel(32,64,64);}
  // else if (current == 101){rainbowCycle(5);}
  // // else if (current == 42) {testCorners();}
  // else if (current == 41) {testLines();}
  // else if (current == 40) {testRainbow();}
  // // if (current == 39) {expandingCircle(250, 250, 250, 15, 10, 3, 50);}
  // else if (current == 39) {flash(250, 250, 250, 10, 40, 300);}
  else if (current == '+') { // '+' = 43
    RPC();
  }
  else if (B('0', current, 'z'+1)) { // 48 - 122
    representation(current);
    current = '!';
  }
}


// ############################################################################# FONCTIONS #############################################################################
// void circleRainbow() {
//   short ledID;
//   for (byte x = 0; x < WIDTH; ++x) {
//     for (byte y = 0; y < HEIGHT; ++y) {
//       if ((ledID = id(x, y)) != -1) {
//         float distanceToCenter = sqrt(pow(x-WIDTH/2, 2) + pow(y-HEIGHT/2, 2));
//         LEDs[ledID] = CRGB(
//           () * distanceToCenter,
//           () * distanceToCenter,
//           () * distanceToCenter);
//       }
//     }
//   }
// }

void RPC() {
  DSERIALln("Welcome in Remote Procedure Call");
  bool done = false;
  byte argCount = 0;
  byte argRequired = 0;
  String args[20];
  char animation = '?';
  while (!done) {
    if (Serial.available() > 0 && !done) {
      byte c = Serial.read();
      // DSERIAL("--------------");
      // DSERIAL(argCount);
      // DSERIAL(" / ");
      // DSERIALln(argRequired);
      // DSERIAL("Got ");
      // DSERIALln((char) c);
      if (animation == '?') {
        if (c == 'q') { // quit
          animation = c;
          argRequired = 0;
          done = true;
        } else if (c == 'g') { // gradient function
          animation = c;
          argRequired = 14;
        } else if(c == 'f') { // flash function
          animation = c;
          argRequired = 6;
        } else if(c == 's') { // spark function
          animation = c;
          argRequired = 8;
        }
      } else if (c == ' ' || c == ',' || c == '\n') {
        if (animation != '?') {
          if (args[argCount] != "") {
            argCount++;
          }
          if (argCount >= argRequired) {
            done = true;
          }
        }
      } else {
        args[argCount] += (char) c;
      }
    }
  }
  SERIAL.readString(); // flush buffer

  DSERIAL("Running animation ");
  DSERIAL(animation);
  for (short i = 0; i < argRequired; ++i) {
    DSERIAL(args[i]); DSERIAL(",");
  }
  SERIAL.println("");
  if (animation == 'q') {
  } else if (animation == 'g') { // ex: g255,255,255,0,0,0,3000,30,0,0,32,0,true,false,
    gradient(CRGB(args[0].toInt(), args[1].toInt(), args[2].toInt()), CRGB(args[3].toInt(), args[4].toInt(), args[5].toInt()),
      args[6].toInt(),
      args[7].toInt(),
      args[8].toInt(), args[9].toInt(),
      args[10].toInt(), args[11].toInt(),
      args[12] == "true" ? true : (args[12] == "false" ? false : args[12].toInt() == 0),
      args[13] == "true" ? true : (args[13] == "false" ? false : args[13].toInt() == 0));
  } else if (animation == 'f') {
    flash(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt(), args[4].toInt(), args[5].toInt());
  } else if (animation == 's') { // ex: s 10,10  5  1000,  0,  0,0,0,
    spark(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt(), args[4].toInt(), CRGB(args[5].toInt(), args[6].toInt(), args[7].toInt()));
  }
}

void representation(char step) {
  DSERIAL("Representation, starting at step ");
  DSERIALln(step);

  #ifndef ONLY_REMOTE_SYNC
    const unsigned long baseBoardTime = millis(); // millis on board, when representation() gets called
    long baseVideoTime = -1; // millis in the video, where the requested step starts (ex: step 2 => 38*1000 millisenconds)
    long delta;
    unsigned long currentBoardTime;
    unsigned long dateBoardTime;
    #define END_OF_CMD()
    #define STARTAT(dateVideoTime)  if (baseVideoTime == -1) { /* In case this is the first time that STARTAT gets called */ \
                                      baseVideoTime = dateVideoTime; \
                                      delta = baseBoardTime - baseVideoTime; \
                                    } \
                                    dateBoardTime = delta + dateVideoTime; \
                                    currentBoardTime = millis(); \
                                    if (dateBoardTime <= currentBoardTime && dateVideoTime != 0) { /* in case the animation took too much time */ \
                                      DSERIAL("late from "); \
                                      DSERIAL(currentBoardTime - dateBoardTime); \
                                      DSERIALln(" ms"); \
                                    } else { \
                                      WasteTime(dateBoardTime - currentBoardTime); \
                                    }

  #else
    #define END_OF_CMD() break;
    #define STARTAT(dateVideoTime)
  #endif

  // #define STEP_COUNTER '0'
  // #define NEW_STEP() STEP_COUNTER
  // #define TESTDELAMORT case '0':

  switch(step) {
    case '0':
      STARTAT(0);
      END_OF_CMD();
    case '1':
      // STARTAT(MSM(0, 38, 000));
      STARTAT(MSM(0, 38, 840));
      flash(255, 255, 255, 1, 560, 560); // flash(r,g,b,beats,duration,period)
      END_OF_CMD();
    case '2':
      // STARTAT(MSM(0, 45, 000));
      STARTAT(MSM(0, 45, 840));
      flash(cyan, 5, 20, 40); // flash(r,g,b,beats,duration,period)

      // STARTAT(MSM(0, 47, 000)); // TODO: add other animations
      END_OF_CMD();
    case 'a':
      STARTAT(MSM(0, 48, 160));
      spark(2,-2,  7, 100,  820,  cyan); // until 49:080
      END_OF_CMD();
    case 'b':
      STARTAT(MSM(0, 49, 080));
      spark(29,-2,  7, 100,  820,  cyan); // until 50:000
      END_OF_CMD();
    case 'c':
      STARTAT(MSM(0, 50, 680));
      gradient(cyan, cyan, 560, 10, // origin, end, duration,stroke,
                                   0, 0, // bx,by,
                                   0, HEIGHT, // ex,ey,
                                   true, false); // lineElseDot,reverse
                                    // until 51:240
      END_OF_CMD();
    case '3':
      STARTAT(MSM(0, 53, 000)); gradient(purple, yellow, 560, 10, // origin, end, duration,stroke,
                                   0, 0, // bx,by,
                                   WIDTH, 0, // ex,ey,
                                   true, false); // lineElseDot,reverse
      END_OF_CMD();
    case '4':
      STARTAT(MSM(0, 53, 560)); gradient(cyan, purple, 1200, 10, // origin, end, duration,stroke,
                                   WIDTH, 0, // bx,by,
                                   0, HEIGHT, // ex,ey,
                                   false, false); // lineElseDot,reverse
      END_OF_CMD();
    case '5':
      STARTAT(MSM(0, 54, 760)); gradient(purple, yellow, 840, 10, // origin, end, duration,stroke,
                                   0, HEIGHT, // bx,by,
                                   WIDTH, 0, // ex,ey,
                                   false, false); // lineElseDot,reverse
      END_OF_CMD();
    case 'd':
      STARTAT(MSM(0, 56, 760));
      spark(20,10,4, 400, 0, cyan);
      spark(10,20,4, 400, 0, yellow);
      spark(20,10,4, 400, 0, orange);
      spark(13,6,4, 400, 0, purple);
      spark(16,10,3, 400, 0, cyan);
      END_OF_CMD();
    case 'e':
      STARTAT(MSM(1, 03, ));
      spark(2,-2,  7, 200,  560,  yellow); // until 57:520
      END_OF_CMD();
    case '6':
      // STARTAT(MSM(1,07,000));
      STARTAT(MSM(1,07,520));
      flash(cyan, 1, 50, 300);// flash(r,g,b,beats,duration,period)
      flash(yellow, 1, 50, 300);// flash(r,g,b,beats,duration,period)
      flash(purple, 1, 50, 300);// flash(r,g,b,beats,duration,period)
      flash(cyan, 1, 50, 300);// flash(r,g,b,beats,duration,period)
      flash(yellow, 1, 50, 300);// flash(r,g,b,beats,duration,period)
      END_OF_CMD();
    case '7':
      // STARTAT(MSM(1, 12, 000));
      STARTAT(MSM(1, 12, 680)); gradient(yellow, cyan, 2000, 10, // origin, end, duration,stroke,
                                   0, 0, // bx,by,
                                   WIDTH, 0, // ex,ey,
                                   false, false); // lineElseDot,reverse
      END_OF_CMD();
    case '8':
      STARTAT(MSM(1, 17, 520)); gradient(yellow, purple, 2400, 10, // origin, end, duration,stroke,
                                   0, 0, // bx,by,
                                   WIDTH, 0, // ex,ey,
                                   false, false); // lineElseDot,reverse
      END_OF_CMD();
    case '9':
      // STARTAT(MSM(1, 21, 000));
      STARTAT(MSM(1, 21, 880));
      // heart(purple, 700);
      spark(13,4,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'A':
      STARTAT(MSM(1, 22, 880));
      // heart(cyan, 700);
      spark(20,10,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'B':
      STARTAT(MSM(1, 23, 880));
      // heart(yellow, 700);
      spark(10,20,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'C':
      STARTAT(MSM(1, 24, 880));
      // heart(purple, 700);
      spark(20,10,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'D':
      STARTAT(MSM(1, 25, 800));
      // heart(cyan, 700);
      spark(13,6,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'E':
      STARTAT(MSM(1, 26, 800));
      // heart(yellow, 700);
      spark(16,10,3, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'F':
      STARTAT(MSM(1, 27, 800));
      // heart(purple, 700);
      spark(10,15,5, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'G':
      STARTAT(MSM(1, 29, 200));
      // heart(cyan, 700);
      spark(20,10,4, 400);
      commit(0, 0, 0);
      END_OF_CMD();
    case 'H':
      // STARTAT(MSM(1,33,000));
      STARTAT(MSM(1,33,640));
      commit(255, 255, 255);
      WasteTime(2000);
      END_OF_CMD();
    case 'I':
      STARTAT(MSM(1,35,600));
      commit(100, 100, 255);
      WasteTime(5000);
      END_OF_CMD();
    case 'J':
      STARTAT(MSM(1,40,000));
      commit(0, 0, 0);
      END_OF_CMD();
  }
  DSERIALln("Representation: Done");
}

void spark(const signed char bx, const signed char by, short radius, const unsigned long duration){
  spark(bx, by, radius, duration, 0, CRGB::Black);
}
void spark(const signed char bx, const signed char by, short radius, const unsigned long duration, const unsigned long persitenceDuration, const CRGB &color){
  const byte transition = 1000 / 30; // 30 fps
  const unsigned long startingTime = millis();
  const unsigned short frames = duration / transition;
  radius *= radius; // use squared value instead of srqt of distance

  unsigned long startingLoopAt;
  FastLED.clear(true);
  for (unsigned short t = 0; t < frames; ++t) { // for each frame of the transition
    startingLoopAt = millis();
    const float progression = (float)(t+1) / (float)(frames);

    // compute the color of each LED
    short ledID;
    for (signed char x = 0; x < WIDTH; ++x) { // TODO: work only in the square of end and origin
      for (signed char y = 0; y < HEIGHT; ++y) {
        if ((ledID = id(x, y)) != -1) { // do not bother doing any computation for pixels that won't be displayed
          // vector and distance (expressed as percentage of the overall length of the animation) between origin and current point
          const signed char cdx = x - bx, cdy = y - by;
          const float distance = (cdx*cdx + cdy*cdy); // use squared value instead of srqt of distance


          // determine if the color should be displayed, based on the frame number
          if (distance < radius * progression) {
            if (LEDs[ledID].r != 0 && LEDs[ledID].g != 0 && LEDs[ledID].b != 0) { // if the color of the LED is not black
              // do nothing, the color has already been computed
            } else {
                if (color.r == 0 && color.g == 0 && color.b == 0) {
                  if (ledID%4 == 0){LEDs[ledID] = cyan;}
                  else if (ledID%4 == 1){LEDs[ledID] = yellow;}
                  else if (ledID%4 == 2){LEDs[ledID] = purple;}
                  else if (ledID%4 == 3){LEDs[ledID] = orange;}
                } else {
                  LEDs[ledID] = color;
                }
            }
          }
        }
      }
    }

    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition);
  }

  // display the frame
  FastLED.show();
  if (FromLoop == 0){ return;}
  WasteTime(duration - (millis() - startingTime) + persitenceDuration);
}

void heart(const byte r, const byte g, const byte b, const unsigned int duration) {
  heart(CRGB(r, g, b), duration);
}
void heart(const CRGB color, const unsigned int duration) {
  const unsigned long startingTime = millis();
  FastLED.clear(true);

  short ledID;
  // bits array method
  const byte bitmap[] =  {
                          0b00011000,
                          0b01100000,
                          0b11010010,
                          0b11000110,
                          0b00110001,
                          0b10110000,
                          0b11000011,
                          0b10000011,
                          0b00000101,
                          0b00000000,
                          0b00100110,
                          0b00000001,
                          0b10000110,
                          0b00011000,
                          0b00001100,
                          0b11000000,
                          0b00001100,
                          0b00000000,
                          0b00110000,
                          // 0b00}; // 14 * 11 pixels
                          0b00000000}; // 14 * 11 pixels
  const byte transition = 1000 / 30; // 30 fps
  const unsigned short frames = duration / transition;
  unsigned long startingLoopAt;
  for (unsigned short t = 0; t < frames; ++t) { // for each frame of the transition
    startingLoopAt = millis();
    const float percentageOfProgressionOfAnimation = (float)(t+1) / (float)(frames);
    for (byte i = 0; i < 154; ++i) {
      if ((bitmap[i/8] & (1<<((7-i%8))) ) != 0) {
        byte x = 9 + i % 14;
        byte y = 3 + i / 14;
        if ((ledID = id(x, y)) != -1) {
          LEDs[ledID].r = color.r * ((9 - pow(percentageOfProgressionOfAnimation*3, 2)) / 9);
          LEDs[ledID].g = color.g * ((9 - pow(percentageOfProgressionOfAnimation*3, 2)) / 9);
          LEDs[ledID].b = color.b * ((9 - pow(percentageOfProgressionOfAnimation*3, 2)) / 9);
        }
      }
    }

    // display the frame
    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition - (millis() - startingLoopAt));
  }

  // display the frame
  FastLED.show();
  if (FromLoop == 0){ return;}
  WasteTime(duration - (millis() - startingTime));
}

// Gradient effect, using the color given by "origin", until the color "end".
// The total duration of the effect will be "duration" milliseconds.
// (bx,by) gives the starting position.
// (ex,ey) gives the ending position.
// TODO: find a way, that the animation goes smoothly until the end
void gradient(const CRGB origin, const CRGB end, const unsigned short duration, const unsigned char stroke, const signed char bx, const signed char by, const signed char ex, const signed char ey, const bool lineElseDot, const bool reverse) {
  const byte transition = 1000 / 30; // 30 fps
  const unsigned short frames = duration / transition;
  const signed char dx = ex - bx, dy = ey - by; // (dx, dy) : overall movement
  const float length = sqrt(dx*dx + dy*dy);
  const float strokeAsPercentage = (float)(stroke) / length;
  // const signed char sdx = dx * strokeAsPercentage, sdy = dy * strokeAsPercentage; // (sdx, sdy) : stroke vector
  const signed short dr = end.r - origin.r, dg = end.g - origin.g, db = end.b - origin.b; // overall RGB color change (dr, dg, db)
  unsigned long startingLoopAt;
  FastLED.clear(true);
  for (unsigned short t = 0; t < frames; ++t) { // for each frame of the transition
    startingLoopAt = millis();
    const float percentageOfProgressionOfAnimation = (float)(t+1) / (float)(frames);
    // (cx,cy) : current position of the front of the wave
    signed char cx = bx + (signed char)(((float)dx * percentageOfProgressionOfAnimation));
    signed char cy = by + (signed char)(((float)dy * percentageOfProgressionOfAnimation));
    // signed char cx = bx + (signed char)(((float)(dx + sdx) * percentageOfProgressionOfAnimation));
    // signed char cy = by + (signed char)(((float)(dy + sdy) * percentageOfProgressionOfAnimation));

    // compute the color of each LED
    short ledID;
    for (signed char x = 0; x < WIDTH; ++x) { // TODO: work only in the square of end and origin
      for (signed char y = 0; y < HEIGHT; ++y) {
        if ((ledID = id(x, y)) != -1) { // do not bother doing any computation for pixels that won't be displayed
          // vector and distance (expressed as percentage of the overall length of the animation) between origin and current point
          const signed short cdx = x - bx, cdy = y - by;

          const float distance = (dx*cdx + dy*cdy) / (length*length); // does a line thanks to scalar product // dirty but works fast
          // Other methods, kept here for the record:
            // float distance = sqrt(cdx*cdx + cdy*cdy) / length; // does a circle
            // float distance = max(cdx, cdy) / (length);// TODO: // does a kind of square
            // float distance = sqrt(dx*cdx + dy*cdy) / (length); // mathematically OK ? goes faster and faster, bigger and bigger. So NO
            // Pythagore
            // make sure, it goes only in the right direction
            // const float vectorLineToPointX = (bx + dx * percentageOfProgressionOfAnimation) - x;
            // const float vectorLineToPointY = (by + dy * percentageOfProgressionOfAnimation) - y;
            // float distanceToLine = sqrt(vectorLineToPointX*vectorLineToPointX + vectorLineToPointY*vectorLineToPointY);
            // float distanceToOrigin = sqrt(cdx*cdx + cdy*cdy);
            // float distance = sqrt(distanceToOrigin*distanceToOrigin - distanceToLine*distanceToLine) / length;


          // determine if the color should be displayed, based on the frame number
          if (B(percentageOfProgressionOfAnimation - strokeAsPercentage, distance, (reverse ? percentageOfProgressionOfAnimation+1 : percentageOfProgressionOfAnimation))) {
            if (LEDs[ledID].r != 0 && LEDs[ledID].g != 0 && LEDs[ledID].b != 0) { // if the color of the LED is not black
              // do nothing, the color has already been computed
            } else {
              // float colorCoefficient = distance;
              float colorCoefficient;
              #define D1 0.40f
              #define V1 0.20f

              #define D2 0.60f
              #define V2 0.80f
              if (distance < D1) {
                colorCoefficient = distance * (V1 / D1);
              } else if (B(D1, distance, D2)) {
                colorCoefficient = (distance - D1) * ((V2-V1) / (D2-D1)) + V1;
                // if (colorCoefficient < 0 + V1;
              } else if (D2 <= distance) {
                colorCoefficient = (distance - D2) * ((1.f-V2) / (1.f-D1)) + V2;
              }
              const CRGB color = CRGB(
                  origin.r + (byte)(dr * colorCoefficient),
                  origin.g + (byte)(dg * colorCoefficient),
                  origin.b + (byte)(db * colorCoefficient));
              if (lineElseDot) { // the wave looks like a moving line
                LEDs[ledID] = color;
              } else { // the wave looks like a moving dot
                const float vectorLineToPointX = (bx + dx * percentageOfProgressionOfAnimation) - x;
                const float vectorLineToPointY = (by + dy * percentageOfProgressionOfAnimation) - y;
                float distanceToLine = sqrt(vectorLineToPointX*vectorLineToPointX + vectorLineToPointY*vectorLineToPointY);
                if (distanceToLine <= stroke/2) {
                  LEDs[ledID] = color;
                }
              }
            }
          }
          else {
            LEDs[ledID] = CRGB(0,0,0);
          }
        }
      }
    }

    // display the frame
    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition - (millis() - startingLoopAt));
  }
  commit(0,0,0); // TODO: replace this with a fading out of the stroke at the end of the animation
}

// @param: x,y : center
// @param: r,g,b : color
void expandingCircle(const byte r, const byte g, const byte b, const byte x, const byte y, const byte stroke, const short transition) {
  FastLED.clear(true);
  char radius = 0;
  short ledID;
  unsigned long startingLoopAt;

  for (byte state = 0; state < 35; ++state) { // TODO: make a while with a good condition
    startingLoopAt = millis();
    // for(short i = 0; i < NUM_LEDS_ALL; i++) { // nice hallow effect
    //   LEDs[i] = CRGB(state, state, state);
    // }
    for(short i = 0; i < NUM_LEDS_ALL; i++) {
      LEDs[i] = CRGB(0, 0, 0);
    }
    for (char dx = -radius; dx <= radius; ++dx) {
      for (char dy = -radius; dy <= radius; ++dy) {
        byte distance = sqrt((dx*dx)+(dy*dy));
        if (radius < stroke){
          if (distance <= radius) {
            if ((ledID = id(x+dx, y+dy)) != -1) {
              LEDs[ledID] = CRGB(r, g, b);
            }
          }
        } else {
          if (B(radius-stroke, distance, radius)) {
            if ((ledID = id(x+dx, y+dy)) != -1) {
              LEDs[ledID] = CRGB(r, g, b);
            }
          }
        }
      }
    }
    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition - (millis() - startingLoopAt));
    ++radius;
  }
}

void flash(const CRGB color, const byte beats, const short duration, const short period) {
  flash(color.r, color.g, color.b, beats, duration, period);
}
void flash(const byte r, const byte g, const byte b, const byte beats, const short duration, const short period) {
  FastLED.clear(true);
  unsigned long startingLoopAt;

  for (byte i = beats; i > 0; --i) {
    startingLoopAt = millis();
    commit(r,g,b);
    WasteTime(duration);
    if (FromLoop == 0) {return;}
    commit(0,0,0);
    WasteTime(period - (millis() - startingLoopAt));
    if (FromLoop == 0) {return;}
    // WasteTime(period - duration); // because duration/2 might be troncated (integers :( )
  }
}

void testCorners() {
  FastLED.clear(true);
  LEDs[id(0, 0)] = CRGB::Red;
  LEDs[id(WIDTH-1, 0)] = CRGB::Red;
  LEDs[id(0, FAT_ARM-1)] = CRGB::Green;
  LEDs[id(WIDTH-1, FAT_ARM-1)] = CRGB::Green;
  FastLED.show();
}
void testRainbow() {
  short led = 0;
  FastLED.clear(true);
  for (byte x = 0; x < WIDTH; ++x) {
    for (byte y = 0; y < HEIGHT; ++y) {
      led = id(x, y);
      if (led != -1) {
        LEDs[led] = CRGB(4*x, 4*y, 0);
      }
    }
  }
  FastLED.show();
}
void testLines() {
  short led = 0;
  FastLED.clear(true);
  for (byte x = 0; x < WIDTH; ++x) {
    for (byte y = 0; y < HEIGHT; ++y) {
      led = id(x, y);
      if (led != -1) {
        LEDs[led] = CRGB(50, 200 * (y%2==1), 200*(y%2==0));
      }
    }
  }
  // if ((led = id(0, 0)) != -1) { LEDs[led] = CRGB::Blue;}
  // else {FastLED.clear();}
  // if ((led = id(1, 1)) != -1) { LEDs[led] = CRGB::Blue;}
  // else {FastLED.clear();}
  FastLED.show();
}
// ############ DEFILE ###############
void defile(unsigned int transition){
  FastLED.clear(true);
  int state = 0;
  for(int i = 0; i < NUM_LEDS_ALL; i++) {
    state++;
    if (state == 1){LEDs[i] = CRGB::Red;}
    if (state == 2){LEDs[i] = CRGB::Green;}
    if (state == 3){LEDs[i] = CRGB::Yellow;}
    if (state == 4){LEDs[i] = CRGB::Blue; state = 0;}

    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition);
  }
}

// ############# RVB WHEEL ################

void rvbWheel(unsigned int r, unsigned int v, unsigned int b){
  for (unsigned int u = v; u < 256; u++)
    {if (FromLoop == 0){ return;}commit(r,u,b);}

  for (unsigned int u = 255; u > v; u--)
    {if (FromLoop == 0){ return;}commit(r,u,b);}

  for (unsigned int u = r; u < 256; u++)
    {if (FromLoop == 0){ return;}commit(u,v,b);}

  for (unsigned int u = 255; u > r; u--)
    {if (FromLoop == 0){ return;}commit(u,v,b);}

  for (unsigned int u = b; u < 256; u++)
    {if (FromLoop == 0){ return;}commit(r,v,u);}

  for (unsigned int u = 255; u > b; u--)
    {if (FromLoop == 0){ return;}commit(r,v,u);}
}

//################################## RainbowCycle ##################################

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS_ALL; i++) {
      c=Wheel(((i * 256 / NUM_LEDS_CHEST) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
      if (FromLoop == 0){ return;}
      ForBreak();
    }
    showStrip();
    WasteTime(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if(WheelPos < 85) {
    c[0]=WheelPos * 3;
    c[1]=255 - WheelPos * 3;
    c[2]=0;
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    c[0]=255 - WheelPos * 3;
    c[1]=0;
    c[2]=WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0]=0;
    c[1]=WheelPos * 3;
    c[2]=255 - WheelPos * 3;
  }
  return c;
}

void showStrip() {
  #ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
  #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  #ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
  #endif
  #ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  LEDs[Pixel].r = red;
  LEDs[Pixel].g = green;
  LEDs[Pixel].b = blue;
  #endif
}





// ############################################################ SECOND ORDER FONCTIONS ##################################################################################

// set all pixels to the given color
void commit(const CRGB color) {
  commit(color.r, color.g, color.b);
}
void commit(unsigned int r, unsigned int v, unsigned int b){

  for(int i = 0; i < NUM_LEDS_ALL; i++) {
    // set our current dot to red, green, and blue
    LEDs[i] = CRGB(r,v,b);
    ForBreak();
  }

  FastLED.show();
  // delay(10);
}

void WasteTime(long interval){
  unsigned long initialMillis = millis();
  if (FromLoop == 0) { return;}
  while (millis() - initialMillis <= interval && FromLoop != 0)
  {
    if (SERIAL.available() > 0)
    {
                // read the incoming byte:
      incomingByte = SERIAL.read();

                // say what you got:
      DSERIAL("I received (DEC): ");
      DSERIALln(incomingByte);
      current = incomingByte;
      FromLoop = 0;
      FastLED.clear(true);
    }
    if (interval <= 0) { // make sure that we don't wait when interval < 0
      return;
    }
  }
}

void ForBreak(){
  if (SERIAL.available() > 0)
  {
                // read the incoming byte:
    incomingByte = SERIAL.read();

                // say what you got:
    DSERIAL("I received (DEC): ");
    DSERIALln(incomingByte);
    current = incomingByte;
    FromLoop = 0;
    FastLED.clear(true);
  }
}

// Brute force address conversion, XY to Hex cells
uint16_t MyRemap(uint16_t y) {

  // Left part of Jacket
  //alpha is the first led jacket number
  unsigned int alpha = NUM_LEDS_ARM_R;

  // offset cote gauche

  if (y >= alpha && y < alpha+7) { return -1; }
  else if (y >= alpha+7 && y < alpha+14) { return y - 7; }

  else if (y >= alpha+14 && y < alpha+20) { return -1; }
  else if (y >= alpha+20 && y < alpha+28) { return y - 13; }

  else if (y >= alpha+27 && y < alpha+33) { return -1; }
  else if (y >= alpha+33 && y < alpha+84) { return y - 18; }

  else if (y >= alpha+84 && y < alpha+86) { return -1; }
  else if (y >= alpha+86 && y < alpha+98) { return y - 20; }

  else if (y >= alpha+98 && y < alpha+102) { return -1; }
  else if (y >= alpha+102 && y < alpha+112) { return y - 24; }

  // offset cote droit

  else if (y >= alpha+112 && y < alpha+116) { return -1; }
  else if (y >= alpha+116 && y < alpha+126) { return y - 28; }

  else if (y >= alpha+126 && y < alpha+128) { return -1; }
  else if (y >= alpha+128 && y < alpha+182) { return y - 30; }

  else if (y >= alpha+182 && y < alpha+187) { return -1; }
  else if (y >= alpha+187 && y < alpha+196) { return y - 35; }

  else if (y >= alpha+196 && y < alpha+202) { return -1; }
  else if (y >= alpha+202 && y < alpha+210) { return y - 41; }

  else if (y >= alpha+210 && y < alpha+217) { return -1; }
  else if (y >= alpha+217 && y < alpha+224) { return y - 48; }

  else{return y;}
}
