#include "mixer.h"

Mixer::Mixer() {
}

Mixer::Mixer(CRGB* base_buffer, CRGB* layer_buffer, CRGB* mixed_buffer) {
  _base = base_buffer;
  _layer = layer_buffer;
  _mixed = mixed_buffer;
}

void Mixer::Mix() {
	// for (int i = 0; i < NUM_LEDS; i++)
	// {
  //   _mixed[i] = blend(_base[i], _layer[i], 128);
	// }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _mixed[i] = _base[i];
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (_layer[i].r > 12 || _layer[i].g > 12 || _layer[i].b > 12)
      _mixed[i] = _layer[i];
  }
}
