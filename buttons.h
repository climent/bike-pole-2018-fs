#ifndef BUTTONS_H
#define BUTTONS_H

class Buttons {
public:
  Buttons(int pin_up, int pin_down);
  int8_t Read();
private:
  int _pin_up;
  int _pin_down;
  int millisBtwnPushes;
  int lastPush;
  int lastUpState;
  int lastDwState;
  int upState;
  int dwState;
  int now;
};

Buttons::Buttons(int pin_up, int pin_down) {
  _pin_up = pin_up;
  _pin_down = pin_down;
  millisBtwnPushes = 100;
  lastPush = millis();
  lastUpState = HIGH;
  lastDwState = HIGH;
}

int8_t Buttons::Read() {
  now = millis();
  upState = digitalRead(_pin_up);
  dwState = digitalRead(_pin_down);

  // Serial.print(lastUpState); Serial.print("  "); Serial.println(upState);
  // Serial.println(dwState);
  if (upState == lastUpState && dwState == lastDwState) return 0;

  if (upState != lastUpState && upState == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      lastPush = millis();
      Serial.println("up");
      lastUpState = upState;
      return 1;
    }
  } else {
    lastUpState = upState;
  }

  if (dwState != lastDwState && dwState == LOW)
  {
    if (now - millisBtwnPushes > lastPush)
    {
      lastPush = millis();
      Serial.println("down");
      lastDwState = dwState;
      return -1;
    }
  } else {
    lastDwState = dwState;
  }
  return 0;
}

#endif
