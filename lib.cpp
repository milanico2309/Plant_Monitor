#include "lib.hpp"
#include "config.hpp"
#include "Arduino.h"
#include "view.hpp"

namespace Lib {

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
    default: return 255;
  }
}

//Reads raw sensor data and generates an average over a defined number of meassurments
int avgRead(uint8_t addr) {
  uint16_t acc = 0;  //stores values for average calculation
  for (uint8_t i = 0; i < AVERAGE_OF; i++) {
    acc += analogRead(addr);            //read input value from sensor
    delay(25);                          //wait a moment
  }
  // integer rounded average
  return (acc + (AVERAGE_OF / 2)) / AVERAGE_OF;
}

//Returns humidity as a percent value, based on a scale between the calibrated MIN/MAX of the sensor.
int getHumidity(const int sensorNum) {
  int raw = avgRead(getSensorPin(sensorNum));
  int span = (int)SENSOR_CALIBRATED_MAX - (int)SENSOR_CALIBRATED_MIN;
  int pct = 100 - ((raw - (int)SENSOR_CALIBRATED_MIN) * 100) / span;  // integer math
  int val = constrain(pct, 0, 99);  //limit value to between 0 and 99%
  return val;
}

//Read all sensors and write results to memory
void readSensorsAndUpdateMemory() {
  for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
    ctx.values[sensorNum] = getHumidity(sensorNum);
  }
}

// Return sensor name stored in flash for an index
const __FlashStringHelper* getSensorName(uint8_t idx) {
  switch (idx) {
#if NUM_SENSORS >= 1
    case 0: return SENSOR_1_ID;
#endif
#if NUM_SENSORS >= 2
    case 1: return SENSOR_2_ID;
#endif
#if NUM_SENSORS >= 3
    case 2: return SENSOR_3_ID;
#endif
    default: return F("?");
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void initCtx() {
    ctx = {
    .values = {0, 0, 0}
    };

    for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
      pinMode(getSensorPin(sensorNum), INPUT);
  }
}

}  // namespace Lib