#include "controller.h"

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  base = baseLeds;
  layer = layerLeds;
  ended = false;
}

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds,
    CRGB* nextBaseLeds, CRGB* nextLayerLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  nextBaseEffect = NULL;
  nextLayerEffect = NULL;
  base = baseLeds;
  layer = layerLeds;
  nextBase = nextBaseLeds;
  nextLayer = nextLayerLeds;
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

bool Controller::Render(int deltaMillis) {
  timeLeftTilRender -= deltaMillis;
  if (timeLeftTilRender <= 0)
	{
		timeLeftTilRender = timeTilRender;
    if (baseEffect != NULL) baseEffect->Render();
    if (layerEffect != NULL) layerEffect->Render();
    if (nextBaseEffect != NULL) nextBaseEffect->Render();
    if (nextLayerEffect != NULL) nextLayerEffect->Render();
    if (baseEffect == NULL && layerEffect == NULL) return false;
    return true;
  }
  return false;
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
  baseEffect->SetBuffer(base);
}

void Controller::SetLayerEffect(Effect* effect) {
  layerEffect = effect;
  layerEffect->SetBuffer(layer);
}

void Controller::SetNextBaseEffect(Effect* effect) {
  nextBaseEffect = effect;
  nextBaseEffect->SetBuffer(nextBase);
}

void Controller::SetNextLayerEffect(Effect* effect) {
  nextLayerEffect = effect;
  nextLayerEffect->SetBuffer(nextLayer);
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

void Controller::Animate(unsigned long mics) {
  if (baseEffect != NULL) baseEffect->Animate(mics);
  if (layerEffect != NULL) layerEffect->Animate(mics);
  if (nextBaseEffect != NULL) nextBaseEffect->Animate(mics);
  if (nextLayerEffect != NULL) nextLayerEffect->Animate(mics);

  // For now, assume that if we have a next Base Effect, we are in transition
  if (nextBaseEffect != NULL) {
    float dt = (float)(mics) / 1000000.0f;
    float fadeIncrement = deltaFade[0] * dt;
    fader[0] += fadeIncrement;
    fraction[0] = (uint8_t)(fader[0] * 255.0f);
    if (fader[0] > 1.0f)
    {
      fader[0] = 1.0f;
      fraction[0] = 255;
      baseEffect = nextBaseEffect;
      baseEffect->SetBuffer(base);
      nextBaseEffect = NULL;
    }

    // Serial.printf("Blending: %d",fraction);
    // Use blend to move toward target palette
    for (int j = 0; j < NUM_LEDS; j++)
    {
      outputBuffer[j] = blend(base[j], nextBase[j], fraction[0]);
    }

  }
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
