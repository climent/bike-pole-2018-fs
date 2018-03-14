#include "palmixer.h"
#include "color_utils.h"
#include "palette_data.h"
#include <FastLED.h>

Palmixer::Palmixer() {
}

void Palmixer::Animate(float dt) {
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

			//              Serial.printf("Blending: %d",fraction);
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

void SetRandomPalette(uint8_t whichSlot, float seconds) {
	curPalette[whichSlot] = finalPalette[whichSlot];
	nextPalette[whichSlot] = randomPalette[whichSlot];

	fader[whichSlot] = 0.0f;
	fraction[whichSlot] = 0;
	deltaFade[whichSlot] = 1.0f / seconds;
	active[whichSlot] = true;
}
