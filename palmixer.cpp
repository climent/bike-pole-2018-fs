// #include "palettes.h"
#include "palmixer.h"

Palmixer::Palmixer(int kNumPalettes, CRGBPalette16* palettes) {
  kNumPalettes = kNumPalettes;
  // // Make sure all palettes are initialized
  // finalPalette[0] = palettes[kSchwarzwald];
  // curPalette[0] = finalPalette[0];
  // nextPalette[0] = curPalette[0];
  //
  // finalPalette[1] = palettes[kMistress];
  // curPalette[1] = finalPalette[1];
  // nextPalette[1] = curPalette[1];
  //
  // finalPalette[2] = palettes[kMadras];
  // curPalette[2] = finalPalette[2];
  // nextPalette[2] = curPalette[2];


}

void Palmixer::Animate(float mics) {
  float dt = (float)(mics) / 1000000.0f;

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
				finalPalette[i][j] = blend(curPalette[i][j], nextPalette[i][j], fraction[i]);
			}
		}
	}
}

void Palmixer::SetNewPalette(uint8_t whichSlot, uint8_t newPal, float seconds) {
	//Serial.printf("Set new palette: %d\n", newPal);
	// guard
	if (newPal >= kNumPalettes) return;

	curPalette[whichSlot] = finalPalette[whichSlot];
	nextPalette[whichSlot] = palettes[newPal];

	fader[whichSlot] = 0.0f;
	fraction[whichSlot] = 0;
	deltaFade[whichSlot] = 1.0f / seconds;
	active[whichSlot] = true;
}
