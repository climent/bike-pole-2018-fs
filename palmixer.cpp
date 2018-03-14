#include "palette_data.h"
#include "palmixer.h"

// Palmixer::Palmixer() {
// }

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

void Palmixer::GenerateGlobalPalettes() {
	palettes[kClouds] = CloudColors_p;
	palettes[kRainbow] = RainbowColors_p;
	palettes[kParty] = PartyColors_p;
	palettes[kPurpleFly] = purplefly_gp;
	palettes[kIris] = iris_gp;
	palettes[kMadras] = madras_gp;
	palettes[kLailah] = lailah1_gp;
	palettes[kHope] = hopegoddess_gp;
	palettes[kCompassion] = angelcompassion_gp;
	palettes[kRepose] = angelrepose_gp;
	palettes[kMermaid] = aquamarinemermaid_gp;
	palettes[kFlame] = flame_gp;
	palettes[kSerendil] = serendil_gp;
	palettes[kDaisyFae] = daisyfae_gp;
	palettes[kFaeCat] = faecat_gp;
	palettes[kFireIce] = fireandice_gp;
	palettes[kHangOn] = hangonfatboy_gp;
	palettes[kMistress] = mistressnight_gp;
	palettes[kOtis] = otis_gp;
	palettes[kScoutie] = scoutie_gp;
	palettes[kSunlitWave] = sunlitwave_gp;
	palettes[kTrove] = trove_gp;
	palettes[kBlackhorse] = blackhorse_gp;
	palettes[kPlumbago] = plumbago_gp;
	palettes[kSchwarzwald] = schwarzwald_gp;
	palettes[kNrwc] = nrwc_gp;
	palettes[kEpochs] = epochs_gp;
	palettes[kSky04] = sky_04_gp;
	palettes[kSky12] = sky_12_gp;
	palettes[kSky34] = sky_34_gp;
	palettes[kSky44] = sky_44_gp;
	palettes[kSky45] = sky_45_gp;
	palettes[kSky05] = sky_05_gp;
	palettes[kSky29] = sky_29_gp;
	palettes[kRosa36] = es_rosa_36_gp;
	palettes[kRosa70] = es_rosa_70_gp;
	palettes[kVintage01] = es_vintage_01_gp;
	palettes[kLandscape27] = es_landscape_27_gp;
	palettes[kJul01] = ib_jul01_gp;
	palettes[kIb63] = ib63_gp;
	palettes[kMango] = mango_mystery_gp;
	palettes[kSprinkles] = sprinkles_gp;

	// Make sure all palettes are initialized
	finalPalette[0] = palettes[kSchwarzwald];
	curPalette[0] = finalPalette[0];
	nextPalette[0] = curPalette[0];

	finalPalette[1] = palettes[kMistress];
	curPalette[1] = finalPalette[1];
	nextPalette[1] = curPalette[1];

	finalPalette[2] = palettes[kMadras];
	curPalette[2] = finalPalette[2];
	nextPalette[2] = curPalette[2];
}
