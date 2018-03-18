#ifndef MOTION_H
#define MOTION_H

// Motion stuff
#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

NXPMotionSense imu;
NXPSensorFusion filter;

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

float minAx = 0.0f;
float maxAx = 0.0f;
float minAy = 0.0f;
float maxAy = 0.0f;
float minAz = 0.0f;
float maxAz = 0.0f;

bool gotBumped = false;
bool gotFlipped = false;
bool gotPositiveTurned = false;
bool gotNegativeTurned = false;

bool rightSideUp = true;
unsigned long micsInOtherOrientation = 0;

void InitMotion() {
	// Motion sensors init
	imu.begin();
	filter.begin(100);
}

void UpdateMotion(unsigned long mics) {
	if (imu.available()) {
		// Read the motion sensors
		imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
		// Update the SensorFusion filter
		filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

	  //  Serial.print("updated motion: ");
 		//  Serial.println(ax);

		if (ax > maxAx) maxAx = ax;
		if (ay > maxAx) maxAy = ay;
		if (az > maxAx) maxAz = az;

		if (ax < minAx) minAx = ax;
		if (ay < minAx) minAy = ay;
		if (az < minAx) minAz = az;

		// gotBumped = false;
		// if (maxAx > 2.0f || maxAy > 2.0f || maxAz > 2.0f)
		// {
		// 	gotBumped = true;
    //   Serial.println("BUMP!");
		// }
    //
		// if (rightSideUp && ax < -0.7)
		// {
		// 	Serial.println("rightSideUp");
		// 	micsInOtherOrientation += mics;
		// }
		// else if (!rightSideUp && ax > 0.7)
		// {
		// 	micsInOtherOrientation += mics;
		// }
		// else
		// 	micsInOtherOrientation = 0;
    //
		// // If we are in the opposite orientation rather vertically for 5 seconds,
		// // change state!
		// if (micsInOtherOrientation > 1000000 * 5)
		// {
		// 	Serial.println("Flipped!\n");
		// 	rightSideUp = !rightSideUp;
		// 	gotFlipped = true;
		// }
	}
}

// motion stuff
float roll, pitch, heading; // Euler orientation
float x, y, z; // vector orientation

void getOrientation(float *roll, float *pitch, float *heading,
                    float *x, float *y, float *z) {
	*roll = filter.getRoll();
	*pitch = filter.getPitch();
	*heading = filter.getYaw();

	float headRads = *heading / (TWO_PI);
	float pitchRads = *pitch / (TWO_PI);

	*x = (float)sin(headRads);
	*y = (float)-(sin(pitchRads) * cos(headRads));
	*z = (float)-(cos(pitchRads) * cos(headRads));
}

void analyzeMotion() {

}

#endif
