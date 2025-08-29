#include "lib.hpp"
#include "config.hpp"
#include "Arduino.h"
#include "view.hpp"

namespace Lib {

const __FlashStringHelper* sensorID[NUM_SENSORS];


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Reads raw sensor data and generates an average over a defined number of meassurments
int avgRead(int addr) {
  int values = 0;  //stores values for average calculation
  for (int i = 0; i < AVERAGE_OF; i++) {
		values += analogRead(addr);            //read input value from sensor
		delay(25);														 //wait a moment
	}
  return (float)values / (float)AVERAGE_OF;  //calculate average
}

//Returns humidity as a percent value, based on a scale between the calibrated MIN/MAX of the sensor.
int getHumidity(const int x) {
  //Power the sensor if applicable
  if (ctx.powerPins[x] != -1) {
    digitalWrite(ctx.powerPins[x], HIGH);  //power the sensor
    delay(10);                             //wait a moment for the sensor to power up
  }
  //Read the sensor
  int val = (100 - (((float)avgRead(ctx.readPins[x]) - SENSOR_CALIBRATED_MIN) / ((SENSOR_CALIBRATED_MAX - SENSOR_CALIBRATED_MIN) / 100.0)));  //calculate percentage of humidity
  //Power down the sensor if applicable
  if (ctx.powerPins[x] != -1) {
    digitalWrite(ctx.powerPins[x], LOW);  //power down the sensor
}
return constrain(val, 0, 100);  //limit value to between 0 and 100%
}

//Read all sensors and write results to memory
void readSensorsAndUpdateMemory() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    ctx.values[i] = getHumidity(i);
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void initCtx() {
    ctx = {
    .powerPins = {SENSOR_1_PPIN, SENSOR_2_PPIN, SENSOR_3_PPIN, SENSOR_4_PPIN, SENSOR_5_PPIN, SENSOR_6_PPIN},
    .readPins = {SENSOR_1_PIN, SENSOR_2_PIN, SENSOR_3_PIN, SENSOR_4_PIN, SENSOR_5_PIN, SENSOR_6_PIN},
    .values = {0, 0, 0, 0, 0, 0 }
    };

    for (int i = 0; i < NUM_SENSORS; i++) {
    if (ctx.powerPins[i] != -1) {
      pinMode(ctx.powerPins[i], OUTPUT);
      digitalWrite(ctx.powerPins[i], LOW);  //make sure sensor is off
    }
    pinMode(ctx.readPins[i], INPUT);
  }
}

//Write human friendly names of sensors to memory.
void initIDArray() {
  View::debugLine(F("Init IDs..."));
#if NUM_SENSORS >= 1
  sensorID[0] = SENSOR_1_ID;
#endif
#if NUM_SENSORS >= 2
  sensorID[1] = SENSOR_2_ID;
#endif
#if NUM_SENSORS >= 3
  sensorID[2] = SENSOR_3_ID;
#endif
#if NUM_SENSORS >= 4
  sensorID[3] = SENSOR_4_ID;
#endif
#if NUM_SENSORS >= 5
  sensorID[4] = SENSOR_5_ID;
#endif
#if NUM_SENSORS >= 6
  sensorID[5] = SENSOR_6_ID;
#endif
  View::debugLine(F("done!"));
}

}  // namespace Lib