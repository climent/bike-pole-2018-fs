#include "effect.h"

Twinkles::Twinkles(int amount, int speed, bool white) {
  _amount = amount;
  _white = white;
  _speed = speed;
  ended = true;
}

Twinkles::Twinkles(int amount, bool white) {
  _amount = amount;
  _white = white;
  _speed = 20;
  ended = true;
}

void Twinkles::Render() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    dst[i] = ledColors[i];
  }
}

void Twinkles::Animate(unsigned long mics) {
  timeToTinkle -= mics;
  if (timeToTinkle < 0) {
    Twinkle();
    timeToTinkle = 5000;
  }
}

String Twinkles::Identify() {
  return "twinkles";
}

void Twinkles::Twinkle() {
  fadeToBlackBy(dst, NUM_LEDS, 1);
  random16_add_entropy(random8());
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    if ( ledState[i] == SteadyDim) {
      // this pixels is currently: SteadyDim
      // so we randomly consider making it start getting brighter
      if ( random16(0, NUM_LEDS) < 1) {
        ledState[i] = GettingBrighter;
      }
    } else if (ledState[i] == GettingBrighter) {
      // this pixels is currently: GettingBrighter
      // so if it's at peak color, switch it to getting dimmer again
      if (ledColors[i] >= peak_color) {
        ledState[i] = GettingDimmerAgain;
      } else {
        // otherwise, just keep brightening it:
        ledColors[i] += CRGB(4, 4, 4);
      }

    } else { // getting dimmer again
      // this pixels is currently: GettingDimmerAgain
      // so if it's back to base color, switch it to steady dim
      if (ledColors[i] <= CRGB(0, 0, 0) ) {
        ledColors[i] = CRGB(0, 0, 0); // reset to exact base color, in case we overshot
        ledState[i] = SteadyDim;
      } else {
        // otherwise, just keep dimming it down:
        ledColors[i] -= CRGB(2, 2, 2);
      }
    }
  }
}
