#include "effect.h"

Config::Config(CRGB* _configLeds) {
  configLeds = _configLeds;
}

String Config::Identify() {
  return "config";
}

void Config::Render() {
  // fadeToBlackBy(dst, NUM_LEDS, 1);
  // for ( uint16_t i = 0; i < length; i++) {
    // dst[i] = color;
  // }  
}

void Config::Output(int index) {
  fadeToBlackBy(configLeds, CONFIG_NUM_LEDS, 1);
  configLeds[index] = CRGB::Red;
}
