#include "button.h"

Button::Button(int pin) {
  _pin = pin;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastState = HIGH;
  pinMode(_pin, INPUT_PULLUP);
  digitalWrite(_pin, HIGH);
}

int8_t Button::Read() {
  now = millis();
  state = digitalRead(_pin);

  if (state == lastState) return 0;

  if (state != lastState && state == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      lastPush = millis();
      Serial.println("button pressed");
      lastState = state;
      return 1;
    }
  } else {
    lastState = state;
  }
  return 0;
}
