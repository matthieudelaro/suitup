#include "FastLED.h"
#include "utils.h"

CRGB ALL[NUM_LEDS_ALL];

void setup() {
  byte x = incomingByte;
  FastLED.addLeds<NEOPIXEL, 4>(ALL, 0, NUM_LEDS_ARM_L);
  FastLED.addLeds<NEOPIXEL, 3>(ALL, NUM_LEDS_ARM_L+NUM_LEDS_CHEST, NUM_LEDS_ARM_R);
  FastLED.addLeds<NEOPIXEL, 2>(ALL, NUM_LEDS_ARM_L, NUM_LEDS_CHEST);

  //For Trouser
  FastLED.addLeds<NEOPIXEL, 6>(ALL, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R, NUM_LEDS_LEG_L);
  FastLED.addLeds<NEOPIXEL, 5>(ALL, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R+NUM_LEDS_LEG_L, NUM_LEDS_LEG_R);
  //For Skirt
  //FastLED.addLeds<NEOPIXEL, 5>(ALL, NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_ARM_R, NUM_LEDS_SKIRT);

  Serial1.begin(9600);

  current = 2;
}

void loop() {
  FastLED.clear(true);
  FromLoop = 1;
  if (Serial1.available() > 0) {
                // read the incoming byte:
    incomingByte = Serial1.read();

                // say what you got:
    Serial1.print("I received (DEC): ");
    Serial1.println(incomingByte);
    current = incomingByte;
    FastLED.clear(true);
  }

  if (current == 97){defile(80);}
  else if (current == 122){rvbWheel(32,64,64);}
  else if (current == 101){rainbowCycle(5);}
  else if (current == 42) {testCorners();}
  else if (current == 41) {testLines();}
  else if (current == 40) {testRainbow();}
  // if (current == 39) {expandingCircle(250, 250, 250, 15, 10, 3, 50);}
  else if (current == 39) {flash(250, 250, 250, 10, 40, 300);}
  else representation(current);
}


// ############################################################################# FONCTIONS #############################################################################
// void circleRainbow() {
//   short ledID;
//   for (byte x = 0; x < WIDTH; ++x) {
//     for (byte y = 0; y < HEIGHT; ++y) {
//       if ((ledID = id(x, y)) != -1) {
//         float distanceToCenter = sqrt(pow(x-WIDTH/2, 2) + pow(y-HEIGHT/2, 2));
//         ALL[ledID] = CRGB(
//           () * distanceToCenter,
//           () * distanceToCenter,
//           () * distanceToCenter);
//       }
//     }
//   }
// }

void representation(short step) {
  unsigned long startingTime;
  const CRGB purple = CRGB(222, 0, 228); // original value: CRGB(222, 69, 228)
  const CRGB yellow = CRGB(232, 246, 0); // original value: CRGB(232, 246, 93)
  const CRGB cyan  = CRGB(0, 250, 243);  // original value: CRGB(105, 250, 243)
  switch(step) {
    case 0:
      WasteTime(1000 * 38); // wait until 0:38 (0 minutes 38 seconds)
    case 1:
      startingTime = millis();
      flash(255, 255, 255, 1, 1000, 1000);
      WasteTime(1000 * (49 - 38) - (millis() - startingTime)); // wait until 0:49
    case 2:
      startingTime = millis();
      flash(93, 221, 221, 3, 40, 300); // blue flash
      WasteTime(1000 * (51 - 49) - (millis() - startingTime)); // wait until 51
    case 3:
      startingTime = millis();
      expandingCircle(93, 221, 221, WIDTH/2, -10, 5, 50); // blue from center, expanding to left and right
      WasteTime(1000 * (53 - 51) - (millis() - startingTime));
    case 4:
      startingTime = millis();
      gradient(cyan, yellow,
        1000,
        5,
        0, 0,
        WIDTH, 0,
        false
      );
      WasteTime(1000 * (54 - 53) - (millis() - startingTime));
    case 5:
      startingTime = millis();
      gradient(cyan, purple,
        1000,
        5,
        WIDTH, 0,
        0, HEIGHT,
        false
      );
      WasteTime(1000 * (55 - 54) - (millis() - startingTime));
    case 6:
      startingTime = millis();
      gradient(purple, yellow,
        1000,
        5,
        0, HEIGHT,
        WIDTH, 0,
        false
      );
      WasteTime(1000 * (56 - 55) - (millis() - startingTime));
    case 7:
      startingTime = millis();
      gradient(purple, yellow,
        1000,
        5,
        0, HEIGHT,
        WIDTH, 0,
        false
      );
      WasteTime(1000 * ((60*1 + 8) - 56) - (millis() - startingTime));
    case 8:
      startingTime = millis();
      flash(255, 255, 255, 8, 100, 300);
      WasteTime(1000 * ((60*1 + 34) - (60*1 + 8)) - (millis() - startingTime));
    case 9:
      startingTime = millis();
      commit(255, 255, 255);
      WasteTime(1000);
      commit(100, 100, 100);
      WasteTime(1000);
      commit(0, 0, 0);
  }
}

// float sigmoid(float x)
// {
//      float exp_value;
//      float return_value;

//      /*** Exponential calculation ***/
//      exp_value = exp(-x);

//      /*** Final sigmoid value ***/
//      return_value = 1 / (1 + exp_value);

//      return return_value;
// }

// double sigmoid(float x) {
//   if (fabs(x) < -3.357e-10) {
//     return x;
//   }
//   else {
//     return atan(x);
//   }
// }


// Gradient effect, using the color given by "origin", until the color "end".
// The total duration of the effect will be "duration" milliseconds.
// (bx,by) gives the starting position.
// (ex,ey) gives the ending position.
void gradient(const CRGB origin, const CRGB end, const unsigned short duration, const unsigned char stroke, const signed char bx, const signed char by, const signed char ex, const signed char ey, const bool reverse) {
  const byte transition = 1000 / 30; // 30 fps
  const unsigned short frames = duration / transition;
  const signed char dx = ex - bx, dy = ey - by; // (dx, dy) : overall movement
  const float length = sqrt(dx*dx + dy*dy);
  const float strokeAsPercentage = (float)(stroke) / length;
  const signed short dr = end.r - origin.r, dg = end.g - origin.g, db = end.b - origin.b; // overall RGB color change (dr, dg, db)
  unsigned long startingLoopAt;
  for (unsigned short t = 0; t < frames; ++t) { // for each frame of the transition
    startingLoopAt = millis();
    const float percentageOfProgressionOfAnimation = (float)(t+1) / (float)(frames); // TODO: use in lines under this
    // (cx,cy) : current position of the front of the wave
    signed char cx = bx + (signed char)(((float)dx * percentageOfProgressionOfAnimation));// TODO: test without cast to float
    signed char cy = by + (signed char)(((float)dy * percentageOfProgressionOfAnimation));// TODO: test without cast to float

    // compute the color of each LED
    short ledID;
    // FastLED.clear(true);
    for (signed char x = 0; x < WIDTH; ++x) { // TODO: work only in the square of end and origin
      for (signed char y = 0; y < HEIGHT; ++y) {
        if ((ledID = id(x, y)) != -1) { // do not bother doing any computation for pixels that won't be displayed
          // vector and distance (expressed as percentage of the overall length of the animation) between origin and current point
          signed short cdx = x - bx, cdy = y - by;

          float distance = (dx*cdx + dy*cdy) / (length*length); // does a line thanks to scalar product // dirty but works fast
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
            // TODO: create feature: make sure, it goes only in the right direction
            // const float vectorLineToPointX = (bx + dx * percentageOfProgressionOfAnimation) - x;
            // const float vectorLineToPointY = (by + dy * percentageOfProgressionOfAnimation) - y;
            // float distanceToLine = sqrt(vectorLineToPointX*vectorLineToPointX + vectorLineToPointY*vectorLineToPointY);
            // if (distanceToLine <= stroke) {
              // float colorCoefficient = max(distance - 0.5, 0.5 - distance)
              // const float colorCoefficient = distance; //sigmoid(distance); // TODO: use some kind of sigmoid
              ALL[ledID] = CRGB(
                origin.r + (byte)(dr * distance),
                origin.g + (byte)(dg * distance),
                origin.b + (byte)(db * distance));
            // }

          }
          else {
            ALL[ledID] = CRGB(0,0,0);
          }
        }
      }
    }

    // display the frame
    FastLED.show();
    if (FromLoop == 0){ return;}
    WasteTime(transition - (millis() - startingLoopAt)); // TODO: here, as well as in all animation: take computation time into account
  }
  commit(0,0,0);
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
    //   ALL[i] = CRGB(state, state, state);
    // }
    for(short i = 0; i < NUM_LEDS_ALL; i++) {
      ALL[i] = CRGB(0, 0, 0);
    }
    for (char dx = -radius; dx <= radius; ++dx) {
      for (char dy = -radius; dy <= radius; ++dy) {
        byte distance = sqrt((dx*dx)+(dy*dy));
        if (radius < stroke){
          if (distance <= radius) {
            if ((ledID = id(x+dx, y+dy)) != -1) {
              ALL[ledID] = CRGB(r, g, b);
            }
          }
        } else {
          if (B(radius-stroke, distance, radius)) {
            if ((ledID = id(x+dx, y+dy)) != -1) {
              ALL[ledID] = CRGB(r, g, b);
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

void flash(const byte r, const byte g, const byte b, const byte beats, const short duration, const short period) {
  FastLED.clear(true);
  unsigned long startingLoopAt;

  for (byte i = beats; i > 0; --i) {
    startingLoopAt = millis();
    commit(r,g,b);
    WasteTime(duration);
    commit(0,0,0);
    WasteTime(period - (millis() - startingLoopAt));
    // WasteTime(period - duration); // because duration/2 might be troncated (integers :( )
  }
}

void testCorners() {
  FastLED.clear(true);
  ALL[id(0, 0)] = CRGB::Red;
  ALL[id(WIDTH-1, 0)] = CRGB::Red;
  ALL[id(0, FAT_ARM-1)] = CRGB::Green;
  ALL[id(WIDTH-1, FAT_ARM-1)] = CRGB::Green;
  FastLED.show();
}
void testRainbow() {
  short led = 0;
  FastLED.clear(true);
  for (byte x = 0; x < WIDTH; ++x) {
    for (byte y = 0; y < HEIGHT; ++y) {
      led = id(x, y);
      if (led != -1) {
        ALL[led] = CRGB(4*x, 4*y, 0);
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
        ALL[led] = CRGB(50, 200 * (y%2==1), 200*(y%2==0));
      }
    }
  }
  // if ((led = id(0, 0)) != -1) { ALL[led] = CRGB::Blue;}
  // else {FastLED.clear();}
  // if ((led = id(1, 1)) != -1) { ALL[led] = CRGB::Blue;}
  // else {FastLED.clear();}
  FastLED.show();
}
// ############ DEFILE ###############
void defile(unsigned int transition){
  FastLED.clear(true);
  int state = 0;
  for(int i = 0; i < NUM_LEDS_ALL; i++) {
    state++;
    if (state == 1){ALL[i] = CRGB::Red;}
    if (state == 2){ALL[i] = CRGB::Green;}
    if (state == 3){ALL[i] = CRGB::Yellow;}
    if (state == 4){ALL[i] = CRGB::Blue; state = 0;}

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
  ALL[Pixel].r = red;
  ALL[Pixel].g = green;
  ALL[Pixel].b = blue;
  #endif
}





// ############################################################ SECOND ORDER FONCTIONS ##################################################################################

// set all pixels to the given color
void commit(unsigned int r, unsigned int v, unsigned int b){

  for(int i = 0; i < NUM_LEDS_ALL; i++) {
    // set our current dot to red, green, and blue
    ALL[i] = CRGB(r,v,b);
    ForBreak();
  }

  FastLED.show();
  delay(10);
}

void WasteTime(long interval){
  unsigned long initialMillis = millis();
  if (FromLoop == 0) { interval = 0;}
  while (millis() - initialMillis <= interval)
  {
    if (Serial1.available() > 0)
    {
                // read the incoming byte:
      incomingByte = Serial1.read();

                // say what you got:
      Serial1.print("I received (DEC): ");
      Serial1.println(incomingByte);
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
  if (Serial1.available() > 0)
  {
                // read the incoming byte:
    incomingByte = Serial1.read();

                // say what you got:
    Serial1.print("I received (DEC): ");
    Serial1.println(incomingByte);
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
