#include "controller.h"

Controller::Controller(CRGB* baseLeds, CRGB* layerLeds) {
  baseEffect = NULL;
  layerEffect = NULL;
  base = baseLeds;
  layer = layerLeds;
  ended = false;
}

String Controller::Identify() {
	return "controller";
}

// void Controller::Initialize() {
// }

void Controller::Render() {
  if (baseEffect != NULL) baseEffect->Render();
  if (layerEffect != NULL) layerEffect->Render();
}

void Controller::Render(CRGBPalette256* finalPalette) {
  if (baseEffect != NULL) baseEffect->Render(finalPalette);
  if (layerEffect != NULL) layerEffect->Render();
}

void Controller::SetEffect(Effect* effect) {
  SetBaseEffect(effect);
}

void Controller::SetBuffer(CRGB* dest) {
  baseEffect->SetBuffer(dest);
}

void Controller::SetBaseEffect(Effect* effect) {
  baseEffect = effect;
  baseEffect->SetBuffer(base);
}

void Controller::SetLayerEffect(Effect* effect) {
  layerEffect = effect;
  layerEffect->SetBuffer(layer);
}

String Controller::GetBaseEffect() {
  if (baseEffect != NULL) return baseEffect->Identify();
  return "";
}

String Controller::GetLayerEffect() {
  if (layerEffect != NULL) return layerEffect->Identify();
  return "";
}

void Controller::Animate(unsigned long mics) {
  if (baseEffect != NULL) baseEffect->Animate(mics);
  if (layerEffect != NULL) layerEffect->Animate(mics);
}

bool Controller::CheckEnd() {
  return baseEffect->CheckEnd();
}

void Controller::Reset() {
  if (baseEffect != NULL) baseEffect->Reset();
  if (layerEffect != NULL) layerEffect->Reset();
}
