#include "controller.h"

Controller::Controller() {
  _effect = NULL;
  ended = false;
}

String Controller::Identify() {
	return "controller";
}

void Controller::SetEffect(Effect* effect) {
  _effect = effect;
}

void Controller::Animate(unsigned long mics) {
	// Serial.println("test");
  _effect->Animate();
}

bool Controller::CheckEnd() {
  return _effect->CheckEnd();
}

void Controller::Reset() {
  _effect->Reset();
}

void Controller::SetBuffer(CRGB* dest) {
  _effect->SetBuffer(dest);
}
