#ifndef LAYOUT_H
#define LAYOUT_H

// Returns the elevation of a single pixel in the string.
float GetSinglePixelElevation(uint16_t elevation) {
	return elevation;
 	if (elevation < 100) return elevation;
	if (elevation >= 100) return 200 - 1 - elevation;
	return elevation;
	if (elevation >= 70 && elevation < 100) return 130 - 1 - elevation;
	if (elevation >= 100 && elevation < 140) return 70 + elevation;
	if (elevation >= 140 && elevation < 170) return elevation - 80;
	if (elevation >= 170 && elevation < 210) return elevation - 170 + 90;
  return 0;
}

#endif
