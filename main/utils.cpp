#include "utils.h"

byte incomingByte = 0; // for incoming Serial1 data
unsigned long previousMillis = 0;
bool FromLoop = 0;
byte current = 97;

// short idArmsUp(signed char x, signed char y) {
// // short id(signed char x, signed char y, byte mode) {
//     // if (mode == 0) {
//         // return id(x, y);
//     // } else if (mode == 1){ // arms up
//         if (B(LENGTH_ARM, x, LENGTH_ARM+FAT_ARM) && B(-LENGTH_ARM, y, 0)) {

//         }
//     // }
// }
short id(signed char x, signed char y) {
    if (!B(0, x, WIDTH) || !B(0, y, HEIGHT)) { // void pixels
        return -1;
    } else if (B(0, x, LENGTH_ARM) && B(0, y, FAT_ARM)) { // left arm (seen from the audience)
        return (NUM_LEDS_ARM_R + NUM_LEDS_ARM_L + NUM_LEDS_CHEST) - 1 - (x + y*LENGTH_ARM);
    } else if (B(LENGTH_ARM+WIDTH_CHEST, x, WIDTH) && B(0, y, FAT_ARM)) { // right arm
        return (x - (WIDTH_CHEST + LENGTH_ARM)) + y * LENGTH_ARM;
    } else if (B(LENGTH_ARM, x, LENGTH_ARM + WIDTH_CHEST) && B(0, y, HEIGHT_CHEST)) { // chest
        short temp = y + HEIGHT_CHEST * (LENGTH_ARM + WIDTH_CHEST - 1 - x);

        // Take into account that LEDs are missing at the top of most columns,
        // due to the shape of the chest
        if (temp < 7) { return -1;} // 7 LEDs are missing at the top of the first column
        else if (B(7, temp, HEIGHT_CHEST)) { temp -= 7; }

        else if (B(HEIGHT_CHEST, temp, HEIGHT_CHEST+6)) { return -1;} // 6 LEDs are missing in 2nd column
        else if (B(HEIGHT_CHEST+6, temp, HEIGHT_CHEST*2)) { temp -= (7+6); }

        else if (B(HEIGHT_CHEST*2, temp, HEIGHT_CHEST*2+5)) { return -1;} // 5 LEDs are missing in 2nd column
        else if (B(HEIGHT_CHEST*2+5, temp, HEIGHT_CHEST*6)) { temp -= (7+6+5); }

        else if (B(HEIGHT_CHEST*6, temp, HEIGHT_CHEST*6+2)) { return -1;} // 2 LEDs ...
        else if (B(HEIGHT_CHEST*6+2, temp, HEIGHT_CHEST*7)) { temp -= (7+6+5+2); }

        else if (B(HEIGHT_CHEST*7, temp, HEIGHT_CHEST*7+4)) { return -1;} // 4 LEDs
        else if (B(HEIGHT_CHEST*7+4, temp, HEIGHT_CHEST*8)) { temp -= (7+6+5+2+4); }

        // ---- center of the chest

        else if (B(HEIGHT_CHEST*8, temp, HEIGHT_CHEST*8+4)) { return -1;} // 4 LEDs
        else if (B(HEIGHT_CHEST*8+4, temp, HEIGHT_CHEST*9)) { temp -= (7+6+5+2+4+4); }

        else if (B(HEIGHT_CHEST*9, temp, HEIGHT_CHEST*9+2)) { return -1;} // 2 LEDs
        else if (B(HEIGHT_CHEST*9+2, temp, HEIGHT_CHEST*13)) { temp -= (7+6+5+2+4+4+2); }

        else if (B(HEIGHT_CHEST*13, temp, HEIGHT_CHEST*13+5)) { return -1;} // 5 LEDs
        else if (B(HEIGHT_CHEST*13+5, temp, HEIGHT_CHEST*14)) { temp -= (7+6+5+2+4+4+2+5); }

        else if (B(HEIGHT_CHEST*14, temp, HEIGHT_CHEST*14+6)) { return -1;} // 6 LEDs
        else if (B(HEIGHT_CHEST*14+6, temp, HEIGHT_CHEST*15)) { temp -= (7+6+5+2+4+4+2+5+6); }

        else if (B(HEIGHT_CHEST*15, temp, HEIGHT_CHEST*15+7)) { return -1;} // 7 LEDs
        else if (B(HEIGHT_CHEST*15+7, temp, HEIGHT_CHEST*16)) { temp -= (7+6+5+2+4+4+2+5+6+7); }

        return temp + NUM_LEDS_ARM_R;
    }
    #ifdef SUIT_FOR_MAN
        else if (B(LENGTH_ARM + WIDTH_CHEST - WIDTH_LEG, x, LENGTH_ARM + WIDTH_CHEST) && B(HEIGHT_CHEST, y, HEIGHT)) { // right leg
            return (y - HEIGHT_CHEST) + HEIGHT_LEG * (LENGTH_ARM + WIDTH_CHEST - 1 - x) + (NUM_LEDS_ARM_R+NUM_LEDS_ARM_L+NUM_LEDS_CHEST);
        } else if (B(LENGTH_ARM, x, LENGTH_ARM + WIDTH_LEG) && B(HEIGHT_CHEST, y, HEIGHT)) { // left leg
            return (y - HEIGHT_CHEST) + HEIGHT_LEG * (LENGTH_ARM + WIDTH_LEG - 1 - x) + (NUM_LEDS_ARM_R+NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_LEG_R);
        }
    #else // ifdef SUIT_FOR_GIRL
        else if (B(SKIRT_BLANK, x, WIDTH) && B(HEIGHT_CHEST, y, HEIGHT) && (x % SKIRT_BLANK == 0)) {
            return (y - HEIGHT_CHEST) + HEIGHT_SKIRT * (WIDTH/SKIRT_BLANK - x/SKIRT_BLANK)  +(NUM_LEDS_ARM_R+NUM_LEDS_ARM_L+NUM_LEDS_CHEST); // TODO: check this
            // return (y - HEIGHT_CHEST) + HEIGHT_LEG * (LENGTH_ARM + WIDTH_LEG - 1 - x) + (NUM_LEDS_ARM_R+NUM_LEDS_ARM_L+NUM_LEDS_CHEST+NUM_LEDS_LEG_R);
        }
    #endif
    else {
        return -1;
    }
}
