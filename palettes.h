#ifndef PALETTES_H
#define PALETTES_H

#include "palette_data.h"

// #define kNumPalettes 64
// int kNumPalettes = 64;

enum PalChoice {
  kClouds = 0,
  kRainbow,
  kParty,
  kPurpleFly,
  kIris,
  kMadras,
  kLailah,
  kHope,
  kCompassion,
  kRepose,
  kMermaid,
  kSerendil,
  kFlame,
  kDaisyFae,
  kFaeCat,
  kFireIce,
  kHangOn,
  kMistress,
  kOtis,
  kScoutie,
  kSunlitWave,
  kTrove,
  kBlackhorse,
  kPlumbago,
  kSchwarzwald,
  kNrwc,
  kEpochs,
  kSky04,
  kSky12,
  kSky34,
  kSky44,
  kSky45,
  kSky05,
  kSky29,
  kRosa36,
  kRosa70,
  kVintage01,
  kLandscape27,
  kJul01,
  kIb63,
  kMango,
  kSprinkles,
  kNumPalettes
};

// An array of palette pointers so we can randomly choose one
// CRGBPalette16 palettes[kNumPalettes];

struct Palettes {
  CRGBPalette16  palettes[kNumPalettes];
  CRGBPalette256 nextPalette[3];
  CRGBPalette256 currentPalette[3];
  CRGBPalette256 finalPalette[3];
};
typedef struct Palettes Palettes;

Palettes palettes;

// This generates in mem copies of all of the progmem palettes that we actually want
// These get upscaled into 256 entry palettes when assigned to nextPalette
void GenerateGlobalPalettes() {
	palettes.palettes[kClouds] = CloudColors_p;
	palettes.palettes[kRainbow] = RainbowColors_p;
	palettes.palettes[kParty] = PartyColors_p;
	palettes.palettes[kPurpleFly] = purplefly_gp;
	palettes.palettes[kIris] = iris_gp;
	palettes.palettes[kMadras] = madras_gp;
	palettes.palettes[kLailah] = lailah1_gp;
	palettes.palettes[kHope] = hopegoddess_gp;
	palettes.palettes[kCompassion] = angelcompassion_gp;
	palettes.palettes[kRepose] = angelrepose_gp;
	palettes.palettes[kMermaid] = aquamarinemermaid_gp;
	palettes.palettes[kFlame] = flame_gp;
	palettes.palettes[kSerendil] = serendil_gp;
	palettes.palettes[kDaisyFae] = daisyfae_gp;
	palettes.palettes[kFaeCat] = faecat_gp;
	palettes.palettes[kFireIce] = fireandice_gp;
	palettes.palettes[kHangOn] = hangonfatboy_gp;
	palettes.palettes[kMistress] = mistressnight_gp;
	palettes.palettes[kOtis] = otis_gp;
	palettes.palettes[kScoutie] = scoutie_gp;
	palettes.palettes[kSunlitWave] = sunlitwave_gp;
	palettes.palettes[kTrove] = trove_gp;
	palettes.palettes[kBlackhorse] = blackhorse_gp;
	palettes.palettes[kPlumbago] = plumbago_gp;
	palettes.palettes[kSchwarzwald] = schwarzwald_gp;
	palettes.palettes[kNrwc] = nrwc_gp;
	palettes.palettes[kEpochs] = epochs_gp;
	palettes.palettes[kSky04] = sky_04_gp;
	palettes.palettes[kSky12] = sky_12_gp;
	palettes.palettes[kSky34] = sky_34_gp;
	palettes.palettes[kSky44] = sky_44_gp;
	palettes.palettes[kSky45] = sky_45_gp;
	palettes.palettes[kSky05] = sky_05_gp;
	palettes.palettes[kSky29] = sky_29_gp;
	palettes.palettes[kRosa36] = es_rosa_36_gp;
	palettes.palettes[kRosa70] = es_rosa_70_gp;
	palettes.palettes[kVintage01] = es_vintage_01_gp;
	palettes.palettes[kLandscape27] = es_landscape_27_gp;
	palettes.palettes[kJul01] = ib_jul01_gp;
	palettes.palettes[kIb63] = ib63_gp;
	palettes.palettes[kMango] = mango_mystery_gp;
	palettes.palettes[kSprinkles] = sprinkles_gp;

	// Make sure all palettes are initialized
	// palettes.finalPalette[0] = palettes.palettes[kSchwarzwald];
	// palettes.currentPalette[0] = palettes.finalPalette[0];
	// palettes.nextPalette[0] = palettes.currentPalette[0];
  //
	// palettes.finalPalette[1] = palettes.palettes[kMistress];
	// palettes.currentPalette[1] = palettes.finalPalette[1];
	// palettes.nextPalette[1] = palettes.currentPalette[1];
  //
	// palettes.finalPalette[2] = palettes.palettes[kMadras];
	// palettes.currentPalette[2] = palettes.finalPalette[2];
	// palettes.nextPalette[2] = palettes.currentPalette[2];
}

#endif
