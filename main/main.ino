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

  current = 39;
}

void loop() {
  // Serial1.println("\n");
  // Serial1.println(NUM_LEDS_ALL);
  // Serial1.println(WIDTH);
  // Serial1.println(HEIGHT);
  // Serial1.println(id(0, 0));
  // ALL[id(0, 0)] = CRGB::Red;
  // FastLED.show();
  WasteTime(2000);
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
  if (current == 122){rvbWheel(32,64,64);}
  if (current == 101){rainbowCycle(5);}
  if (current == 42) {testCorners();}
  if (current == 41) {testLines();}
  if (current == 40) {testRainbow();}
  if (current == 39) {expandingCircle(250, 250, 250, 15, 10, 3, 50);}
}


// ############################################################################# FONCTIONS #############################################################################
// @param: x,y : center
// @param: r,g,b : color
void expandingCircle(const byte r, const byte g, const byte b, const byte x, const byte y, const byte stroke, const short transition) {
  FastLED.clear(true);
  char radius = 0;
  short ledID;

  for (byte state = 0; state < 25; ++state) { // TODO: make a while with a good condition
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
    WasteTime(transition);
    ++radius;
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
