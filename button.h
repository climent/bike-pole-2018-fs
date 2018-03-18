#include "includes.h"
#include <FastLED.h>

#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
  Button(int pin);
  int8_t Read();
private:
  int _pin;
  int millisBtwnPushes;
  int lastPush;
  int lastState;
  int state;
  int now;
};

#endif
