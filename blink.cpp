#include "effect.h"

Blink::Blink(int ledPin) {
  _ledPin = ledPin;
  now = millis();
  ledOn = false;
  timeToChange = 500;
  lastChange = millis();

  pinMode(ledPin, OUTPUT);

}

void Blink::Animate(){
  now = millis();
  if (now - lastChange > timeToChange) {
  if (!ledOn){
    digitalWrite(_ledPin, HIGH);   // set the LED on
    lastChange = millis();
    ledOn = !ledOn;
  } else {
    digitalWrite(_ledPin, LOW);    // set the LED off
    lastChange = millis();
    ledOn = !ledOn;
  }
 }
}

String Blink::Identify() {
  return "blink";
}
