// This file is designed to run unit tests.
// Arduino editor will ignore it, since it has a .c++ extension.
// But this is not a problem for g++

#include "utils.h"
#include "gtest/gtest.h"
#include "cmath"
using namespace std;

namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
    // You can do set-up work for each test here.
  }

  virtual ~FooTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(FooTest, idFunction) {
    // Test left arm
    for (int i = 0; i < 8; i++) EXPECT_EQ(id(i, 0), (NUM_LEDS_ARM_R + NUM_LEDS_ARM_L + NUM_LEDS_CHEST) - 1 - i);
    for (int i = 0; i < 8; i++) EXPECT_EQ(id(i, 1), (NUM_LEDS_ARM_R + NUM_LEDS_ARM_L + NUM_LEDS_CHEST) - 1 - i - LENGTH_ARM);

    // test right arm
    EXPECT_EQ(id(WIDTH-1, 0), 7);
    EXPECT_EQ(LENGTH_ARM + WIDTH_CHEST, 24);
    EXPECT_EQ(id(24, 0), 0);
    EXPECT_EQ(WIDTH, 32);
    EXPECT_EQ(id(32-1, 2), 23);
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            EXPECT_EQ(id(x + LENGTH_ARM + WIDTH_CHEST, y), x + y*8);
        }
    }

    // test chest
    EXPECT_EQ(NUM_LEDS_ARM_R - 1, 23);
    EXPECT_EQ(id(23, 6), -1);
    EXPECT_EQ(id(23, 7), 24);
    EXPECT_EQ(id(8, 13), NUM_LEDS_CHEST + NUM_LEDS_ARM_R - 1);

    // test right leg
    EXPECT_EQ(id(LENGTH_ARM + WIDTH_CHEST - 1, HEIGHT_CHEST), NUM_LEDS_ARM_R + NUM_LEDS_ARM_L + NUM_LEDS_CHEST);
    EXPECT_EQ(id(23, 14), 224); // top right corner of the right leg

    // test left leg
    EXPECT_EQ(id(LENGTH_ARM + WIDTH_LEG - 1, HEIGHT_CHEST), NUM_LEDS_ARM_R + NUM_LEDS_ARM_L + NUM_LEDS_CHEST + NUM_LEDS_LEG_R);
    // top right conier of the left leg

    // test void
    EXPECT_EQ(id(0, 3), -1); // under arm
    EXPECT_EQ(id(16, 15), -1); // under chest, between legs

    // for (byte x = 0; x < WIDTH; ++x) {
    //   for (byte y = 0; y < HEIGHT; ++y) {
    //     short led = id(x, y);
    //     if (led != -1) {
    //       cout << led << endl;
    //     }
    //   }
    // }

    cout << "WxH:" << WIDTH << "x" << HEIGHT << endl;
}

struct CRGB {
  byte r, g, b;

  CRGB(byte r, byte g, byte b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }
};



// TEST_F(FooTest, testGradient) {
//     // gradient(CRGB(222, 69, 228), CRGB(232, 246, 93),
//     gradient(CRGB(0, 0, 0), CRGB(0, 0, 255),
//         1000,
//         5,
//         0, HEIGHT/2,
//         WIDTH/2, HEIGHT/2
//         // -10, HEIGHT/2,
//         // WIDTH + 10, HEIGHT/2
//       ); // purpul to yellow, left to right
// }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
