#ifndef EEPROM_H
#define EEPROM_H

// #include <EEPROM.h>                                  // required for reading/saving settings to eeprom

/* these values will be saved to EEPROM:
  0 = index for selected brightness level
  1 = index for selecing the initial effect
  2 = bool for effect transition
  3 = fadingMode
*/

int ReadEEPROM(int index) {
	int currentIndex;
	uint8_t tmp = EEPROM.read(index);
	if ( tmp >= 0 && tmp < 24 ) {
		currentIndex = tmp;
		#ifdef DEBUG
			Serial.print(F("ReadEEPROM(): loaded EEPROM index "));
			Serial.print(index);
			Serial.print(F("("));
			Serial.print(tmp);
			Serial.println(F(")"));
		#endif
	} else {
		currentIndex = 0;
		#ifdef DEBUG
			Serial.print(F("ReadEEPROM(): EEPROM value out of range for index "));
			Serial.print(index);
      Serial.print(F("("));
			Serial.print(tmp);
      Serial.print(F("), loading default value "));
			Serial.println(currentIndex);
		#endif
	}
  return currentIndex;
}

void WriteEEPROM(int index, int value) {
  if (ReadEEPROM(index) != value) {                             // only save selected palette to eeprom if clock is in normal running mode, not while in startup/setup/whatever
    EEPROM.put(index, value);
		// #ifdef NODEMCU
		// 	EEPROM.commit();
		// #endif
    #ifdef DEBUG
      Serial.print(F("WriteEEPROM(): saved EEPROM index "));
      Serial.print(index);
      Serial.print(F("("));
      Serial.print(value);
      Serial.println(F(")"));
    #endif
    delay(500);
  }
}

#endif
