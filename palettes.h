#ifndef PALETTES_H
#define PALETTES_H

#include "color_utils.h"
#include "palette_data.h"

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
//#define kNumPalettes 64
CRGBPalette16 palettes[kNumPalettes];

// when we want to blend to a new palette, we assign to nextPalette, then use nblend to modify the target palette over time.
// all color fetch functions use the final palette in some way or another
CRGBPalette256 nextPalette[3];
CRGBPalette256 curPalette[3];
CRGBPalette256 finalPalette[3];

CRGBPalette16 randomPalette[3];

void GenerateRandomPalettes()
{
    for( int i = 0; i < 16; i++)
    {
        randomPalette[0][i] = CHSV( random8(), random8()/2 + 128, random8());
        randomPalette[1][i] = CHSV( random8(), random8()/2 + 128, random8());
        randomPalette[2][i] = CHSV( random8(), random8()/2 + 128, random8());
    }
}

// This generates in mem copies of all of the progmem palettes that we actually want
// These get upscaled into 256 entry palettes when assigned to nextPalette
void GenerateGlobalPalettes()
{
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
  curPalette[0]  = finalPalette[0] ;
  nextPalette[0]  = curPalette[0] ;

  finalPalette[1] = palettes[kMistress];
  curPalette[1]  = finalPalette[1] ;
  nextPalette[1]  = curPalette[1] ;

  finalPalette[2] = palettes[kMadras];
  curPalette[2]  = finalPalette[2] ;
  nextPalette[2]  = curPalette[2] ;
}




#endif


