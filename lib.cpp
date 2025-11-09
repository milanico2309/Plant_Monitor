/**
 * @file lib.cpp
 * @brief Implementation of sensor reading and runtime context for Plant Monitor.
 */
#include "lib.hpp"
#include "config.hpp"
#include "Arduino.h"
#include "view.hpp"

namespace Lib {

SensorContext ctx;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Resolve the analog pin for a given sensor index.
 * @param sensorIndex Index starting at 0.
 * @return The Arduino analog pin number or 255 if out of range.
 */
uint8_t getSensorPin(uint8_t sensorIndex) {
  switch(sensorIndex) {
    case 0: return SENSOR_1_PIN;
    case 1: return SENSOR_2_PIN;
    case 2: return SENSOR_3_PIN;
    default: return 255;
  }
}

/**
 * @brief Read a sensor multiple times and return the integer-averaged value.
 * @param addr Analog pin address.
 * @return Rounded integer average of @ref AVERAGE_OF samples.
 */
int avgRead(uint8_t addr) {
  uint16_t acc = 0;  //stores values for average calculation
  for (uint8_t i = 0; i < AVERAGE_OF; i++) {
    acc += analogRead(addr);            //read input value from sensor
    delay(25);                          //wait a moment
  }
  // integer rounded average
  return (acc + (AVERAGE_OF / 2)) / AVERAGE_OF;
}

/**
 * @brief Convert a raw sensor reading to a humidity percentage (0–99).
 *
 * Uses the calibrated range @ref SENSOR_CALIBRATED_MIN to
 * @ref SENSOR_CALIBRATED_MAX and clamps to [0, 99].
 * @param sensorNum Sensor index (0-based).
 * @return Percentage humidity value.
 */
int getHumidity(const int sensorNum) {
  int raw = avgRead(getSensorPin(sensorNum));
  int span = (int)SENSOR_CALIBRATED_MAX - (int)SENSOR_CALIBRATED_MIN;
  int pct = 100 - ((raw - (int)SENSOR_CALIBRATED_MIN) * 100) / span;  // integer math
  int val = constrain(pct, 0, 99);  //limit value to between 0 and 99%
  return val;
}

/**
 * @brief Read all sensors and write results to the global context @ref ctx.
 */
void readSensorsAndUpdateMemory() {
  for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
    ctx.values[sensorNum] = getHumidity(sensorNum);
  }
}

/**
 * @brief Return sensor name stored in flash for an index.
 * @param idx 0-based sensor index.
 * @return Pointer to flash-stored name; "?" if out of range.
 */
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

/**
 * @brief Initialize the context and configure sensor input pins.
 */
void initCtx() {
    ctx = {
    .values = {0, 0, 0}
    };

    for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
      pinMode(getSensorPin(sensorNum), INPUT);
  }
}

}  // namespace Lib