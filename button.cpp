#include "button.h"

Button::Button(int pin) {
  _pin = pin;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastState = HIGH;
  pinMode(_pin, INPUT_PULLUP);
  digitalWrite(_pin, HIGH);
}

bool Button::Read() {
  now = millis();
  state = digitalRead(_pin);

  if (state == lastState) return false;

  if (state != lastState && state == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      lastPush = millis();
      if (DEBUG) Serial.println("button pressed");
      lastState = state;
      return true;
    }
  } else {
    lastState = state;
  }
  return false;
}
