#include "controller.h"

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  baseBuffer = baseLeds;
  layerBuffer = layerLeds;
  ended = false;
}

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds,
    CRGB* nextBaseLeds, CRGB* nextLayerLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  nextBaseEffect = NULL;
  nextLayerEffect = NULL;
  baseBuffer = baseLeds;
  layerBuffer = layerLeds;
  nextBaseBuffer = nextBaseLeds;
  nextLayerBuffer = nextLayerLeds;
  ended = false;
}

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds,
    CRGB* nextBaseLeds, CRGB* nextLayerLeds, CRGB* outputLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  nextBaseEffect = NULL;
  nextLayerEffect = NULL;
  baseBuffer = baseLeds;
  layerBuffer = layerLeds;
  nextBaseBuffer = nextBaseLeds;
  nextLayerBuffer = nextLayerLeds;
  outputBuffer = outputLeds;
  ended = false;
}

String Controller::Identify() {
	return "controller";
}

// void Controller::Initialize() {
// }

void Controller::SetTimer(int timer) {
  timeTilRender = timer;
}

void Controller::PreRender(int millis) {
  // Before the actual render occurs we need to decide which effect goes where
  // if (nextBaseEffect != NULL) {
  if (transitionActive) {
    float dt = (float)(millis) / 1000.0f;
    float fadeIncrement = deltaFade[0] * dt;
    fader[0] += fadeIncrement;
    fraction[0] = (uint8_t)(fader[0] * 255.0f);
    if (fader[0] > 1.0f)  {
      // Serial.printf("Fader final value: %f", fader[0]);
      // Serial.println("");
      // for (int j = 0; j < NUM_LEDS; j++) {
      //   outputBuffer[j] = nextBaseEffect->GetBuffer()[j];
      // }
      fader[0] = 0.0f;
      fraction[0] = 0;
      // Transition the data from the next buffer to the current buffer
      for ( uint16_t i = 0; i < NUM_LEDS; i++) {
        baseBuffer[i] = nextLayerBuffer[i];
      }
      SetBaseEffect(nextBaseEffect);
      nextBaseEffect = NULL;
      transitionActive = false;
      Serial.println("Blending done...");
    }
  }
}

bool Controller::Render(int deltaMillis) {
  timeLeftTilRender -= deltaMillis;
  bool response = false;

  PreRender(deltaMillis);

  if (timeLeftTilRender <= 0)	{
		timeLeftTilRender = timeTilRender;
    if (baseEffect != NULL) baseEffect->Render();
    if (layerEffect != NULL) layerEffect->Render();
    if (nextBaseEffect != NULL) nextBaseEffect->Render();
    if (nextLayerEffect != NULL) nextLayerEffect->Render();

    if (baseEffect == NULL && layerEffect == NULL) return false;
    response = true;
  }

  Mix();

  return response;
}

void Controller::Mix() {
  if (transitionActive) {
    // Serial.printf("Blending: %d", fraction[0]);
    // Serial.printf("Fading: %f", fader[0]);
    // Serial.println("");
    // Use blend to move toward target palette
    for (int j = 0; j < NUM_LEDS; j++)  {
      outputBuffer[j] = blend(
          CRGB::Black, baseEffect->GetBuffer()[j], 255 - fraction[0]);
      outputBuffer[j] = blend(
          outputBuffer[j],
          nextBaseEffect->GetBuffer()[j],
          fraction[0]);
    }
  } else {
    for (int j = 0; j < NUM_LEDS; j++) {
      outputBuffer[j] = baseEffect->GetBuffer()[j];
    }
  }
}

void Controller::SetEffect(Effect* effect) {
  SetBaseEffect(effect);
}

void Controller::SetBuffer(CRGB* dest) {
  baseEffect->SetBuffer(dest);
}

void Controller::SetOutputBuffer(CRGB* dest) {
  outputBuffer = dest;
}

void Controller::SetBaseEffect(Effect* effect) {
  baseEffect = effect;
  baseEffect->SetBuffer(baseBuffer);
}

void Controller::SetLayerEffect(Effect* effect) {
  layerEffect = effect;
  layerEffect->SetBuffer(layerBuffer);
}

void Controller::SetNextBaseEffect(Effect* effect) {
  nextBaseEffect = effect;
  nextBaseEffect->SetBuffer(nextBaseBuffer);
}

void Controller::SetNextLayerEffect(Effect* effect) {
  nextLayerEffect = effect;
  nextLayerEffect->SetBuffer(nextLayerBuffer);
}

String Controller::GetBaseEffect() {
  if (baseEffect != NULL) return baseEffect->Identify();
  return "";
}

String Controller::GetLayerEffect() {
  if (layerEffect != NULL) return layerEffect->Identify();
  return "";
}

String Controller::GetNextBaseEffect() {
  if (nextBaseEffect != NULL) return nextBaseEffect->Identify();
  return "";
}

String Controller::GetNextLayerEffect() {
  if (nextLayerEffect != NULL) return nextLayerEffect->Identify();
  return "";
}

void Controller::InitiateTransition(Effect* effect) {
  SetNextBaseEffect(effect);
  transitionActive = true;
}

bool Controller::IsTransitionActive() {
  return transitionActive;
}

void Controller::Animate(unsigned long mics) {
  if (baseEffect != NULL) baseEffect->Animate(mics);
  if (layerEffect != NULL) layerEffect->Animate(mics);
  if (nextBaseEffect != NULL) nextBaseEffect->Animate(mics);
  if (nextLayerEffect != NULL) nextLayerEffect->Animate(mics);
}

bool Controller::CheckEnd() {
  if (baseEffect != NULL) return baseEffect->CheckEnd();
  return true;
}

void Controller::Reset() {
  if (baseEffect != NULL) baseEffect->Reset();
  if (layerEffect != NULL) layerEffect->Reset();
  if (nextBaseEffect != NULL) nextBaseEffect->Reset();
  if (nextLayerEffect != NULL) nextLayerEffect->Reset();
}
