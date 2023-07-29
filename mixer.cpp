#include "mixer.h"

Mixer::Mixer() {
}

Mixer::Mixer(CRGB* base_buffer, CRGB* layer_buffer, CRGB* mixed_buffer) {
  _base = base_buffer;
  _layer = layer_buffer;
  mixedBuffer = mixed_buffer;
}

Mixer::Mixer(CRGB* mixed_buffer) {
  mixedBuffer = mixed_buffer;
}

void Mixer::SetCurrentBuffer(CRGB* current_buffer) {
  currentBuffer = current_buffer;
}

CRGB* Mixer::GetCurrentBuffer() {
  return currentBuffer;
}

bool Mixer::IsActive() {
  return active;
}

void Mixer::Mix() {
  int kMixMode = 1;
  switch (kMixMode) {
    case 1:
      // Put the base in the mixed buffer, add any LED from the layer that's not
      // black.
      for (int i = 0; i < NUM_LEDS; i++)
      {
        mixedBuffer[i] = _base[i];
      }
      for (int i = 0; i < NUM_LEDS; i++)
      {
        if (_layer[i].r > 12 || _layer[i].g > 12 || _layer[i].b > 12)
          mixedBuffer[i] = _layer[i];
      }
      break;
    case 2:
      for (int i = 0; i < NUM_LEDS; i++)
      {
        mixedBuffer[i] = blend(_base[i], _layer[i], 128);
      }
      break;
  }
}

void Mixer::Animate(float mics) {
  float dt = (float)(mics) / 1000000.0f;
  if (active) {
    float fadeIncrement = deltaFade * dt;
    fraction = (uint8_t)(fader * 255.0f);
    fader += fadeIncrement;
    if (fader > 1) {
      fader = 1.0f;
      active = false;
      currentBuffer = nextBuffer;
    }
    for (int j = 0; j < NUM_LEDS; j++){
      mixedBuffer[j] = blend(currentBuffer[j], nextBuffer[j], fraction);
    }
  } else {
    for (int j = 0; j < NUM_LEDS; j++){
      mixedBuffer[j] = currentBuffer[j];
    }
  }
}

void Mixer::MixToPrimary(CRGB* buffer, int seconds) {
  nextBuffer = buffer;
  deltaFade = 1.0f / seconds;
  active = true;
}
