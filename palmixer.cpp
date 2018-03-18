#include "palmixer.h"

Palmixer::Palmixer(int kNumPalettes, CRGBPalette16* palettes,
    CRGBPalette256* nextPalette,
    CRGBPalette256* currentPalette,
    CRGBPalette256* finalPalette) {
  _kNumPalettes = kNumPalettes;
  _palettes = palettes;
  _nextPalette = nextPalette;
  _currentPalette = currentPalette;
  _finalPalette = finalPalette;
  active[0] = false;
  active[1] = false;
  active[2] = false;
  // GenerateGlobalPalettes();
}

void Palmixer::Animate(float mics) {
  float dt = (float)(mics) / 1000000.0f;

  // Serial.println("Blending...");
	for (int i = 0; i < 3; i++)
	{
		if (active[i])
		{
			// For now just one animate mode, crossfade
			float fadeIncrement = deltaFade[i] * dt;
			fader[i] += fadeIncrement;
			fraction[i] = (uint8_t)(fader[i] * 255.0f);
			if (fader[i] > 1.0f)
			{
				fader[i] = 1.0f;
				fraction[i] = 255;
				active[i] = false;
			}

			// Serial.printf("Blending: %d",fraction);
			// Use blend to move toward target palette
			for (int j = 0; j < 256; j++)
			{
				_finalPalette[i][j] = blend(_currentPalette[i][j], _nextPalette[i][j], fraction[i]);
			}
		}
	}
}

void Palmixer::SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds) {
	if (newPal >= 42) return;
  // Serial.printf("Setting new palette: slot[%d] -> [%d]\n", whichSlot, newPal);

	_currentPalette[whichSlot] = _finalPalette[whichSlot];
	_nextPalette[whichSlot] = _palettes[newPal];

	fader[whichSlot] = 0.0f;
	fraction[whichSlot] = 0;
	deltaFade[whichSlot] = 1.0f / seconds;
	active[whichSlot] = true;
}
