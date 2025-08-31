#include "lib.hpp"
#include "config.hpp"
#include "Arduino.h"
#include "view.hpp"

namespace Lib {

const __FlashStringHelper* sensorID[NUM_SENSORS];
SensorContext ctx;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Analog-Pins
//We use this instead of an array to keep the memory footprint low (keep things in PROGMEM)
uint8_t getSensorPin(uint8_t sensorIndex) {
  switch(sensorIndex) {
    case 0: return SENSOR_1_PIN;
    case 1: return SENSOR_2_PIN;
    case 2: return SENSOR_3_PIN;
    case 3: return SENSOR_4_PIN;
    case 4: return SENSOR_5_PIN;
    case 5: return SENSOR_6_PIN;
    default: return 255;
  }
}

// Power-Pins
//We use this instead of an array to keep the memory footprint low (keep things in PROGMEM)
uint8_t getPowerPin(uint8_t sensorIndex) {
  switch(sensorIndex) {
    case 0: return SENSOR_1_PPIN;
    case 1: return SENSOR_2_PPIN;
    case 2: return SENSOR_3_PPIN;
    case 3: return SENSOR_4_PPIN;
    case 4: return SENSOR_5_PPIN;
    case 5: return SENSOR_6_PPIN;
    default: return 255;
  }
}

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
int getHumidity(const int sensorNum) {
  //Power the sensor if applicable
  if (getPowerPin(sensorNum) != -1) {
    digitalWrite(getPowerPin(sensorNum), HIGH);  //power the sensor
    delay(10);                             //wait a moment for the sensor to power up
  }
  //Read the sensor
  int val = (100 - (((float)avgRead(getSensorPin(sensorNum)) - SENSOR_CALIBRATED_MIN) / ((SENSOR_CALIBRATED_MAX - SENSOR_CALIBRATED_MIN) / 100.0)));  //calculate percentage of humidity
  //Power down the sensor if applicable
  if (getPowerPin(sensorNum) != -1) {
    digitalWrite(getPowerPin(sensorNum), LOW);  //power down the sensor
}
return constrain(val, 0, 99);  //limit value to between 0 and 99%
}

//Read all sensors and write results to memory
void readSensorsAndUpdateMemory() {
  for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
    ctx.values[sensorNum] = getHumidity(sensorNum);
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void initCtx() {
    ctx = {
    .values = {0, 0, 0, 0, 0, 0 }
    };

    for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
      if (getPowerPin(sensorNum) != -1) {
        pinMode(getPowerPin(sensorNum), OUTPUT);
        digitalWrite(getPowerPin(sensorNum), LOW);  //make sure sensor is off
      }
      pinMode(getSensorPin(sensorNum), INPUT);
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