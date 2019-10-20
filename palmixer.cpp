#include "palmixer.h"
#define FASTLED_INTERNAL 1

Palmixer::Palmixer(
		CRGBPalette16* palettes,
    CRGBPalette256* currentPalette,
    CRGBPalette256* nextPalette,
    CRGBPalette256* finalPalette) {
  _palettes = palettes;
  _nextPalette = nextPalette;
  _currentPalette = currentPalette;
  _finalPalette = finalPalette;
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

void Palmixer::SetTimer(long timeTilPalChange) {
	timer = timeTilPalChange;
}

void Palmixer::SetTransitionTimer(float transitionTimer) {
	seconds = transitionTimer;
}

void Palmixer::SetDefaultPalette(int _defaultPalette) {
	defaultPalette = _defaultPalette;
}

CRGBPalette256 Palmixer::GetPalette(int8_t index) {
	return _finalPalette[index];
}

CRGBPalette256 Palmixer::GetPalette() {
	return GetPalette(defaultPalette);
}

int Palmixer::GetPaletteIndex() {
	return defaultPalette;
}

void Palmixer::SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds) {
  _kNumPalettes = *(&_palettes + 1) - _palettes;
  // Serial.println(_kNumPalettes);
	if (newPal >= _kNumPalettes) return;
  // Serial.printf("Setting new palette: slot[%d] -> [%d]\n", whichSlot, newPal);

	_currentPalette[whichSlot] = _finalPalette[whichSlot];
	_nextPalette[whichSlot] = _palettes[newPal];

	fader[whichSlot] = 0.0f;
	fraction[whichSlot] = 0;
	deltaFade[whichSlot] = 1.0f / seconds;
	active[whichSlot] = true;
}

void Palmixer::UpdatePalettes(int deltaMicros) {
  _kNumPalettes = *(&_palettes + 1) - _palettes;
	timeLeftTilPalChange -= deltaMicros;
	if (timeLeftTilPalChange <= 0)
	{
		// Serial.printf("Changing palettes...\n");
		int newpal;
		newpal = random(0, _kNumPalettes);
		SetNewPalette(0, newpal, seconds);
		newpal = random(0, _kNumPalettes);
		SetNewPalette(1, newpal, seconds);
		newpal = random(0, _kNumPalettes);
		SetNewPalette(2, newpal, seconds);
		timeLeftTilPalChange = timer;
	}

}
