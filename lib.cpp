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

/// @brief Get PIN id for a given sensor index. Values are defined in config.hpp
/// @param sensorIndex Index of the sensor in config.hpp(0 to NUM_SENSORS-1)
/// @return Sensor PIN id or 255 if index is out of range
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

/// @brief Get PIN id for a given sensor index. Values are defined in config.hpp
/// @param sensorIndex Index of the sensor in config.hpp(0 to NUM_SENSORS-1)
/// @return Sensor PIN id or 255 if index is out of range
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

/// @brief Reads analog value from a given pin multiple times and returns the average. Parameter AVERAGE_OF from config.hpp defines how many readings are taken.
/// @param addr PIN to read from
/// @return The average reading
int avgRead(int addr) {
  int values = 0;  //stores values for average calculation
  for (int i = 0; i < AVERAGE_OF; i++) {
		values += analogRead(addr);            //read input value from sensor
		delay(25);														 //wait a moment
	}
  return (float)values / (float)AVERAGE_OF;  //calculate average
}

/// @brief Reads the humidity from a given sensor. If a power pin is defined for the sensor it will be powered on before reading and powered off afterwards. SENSOR_CALIBRATED_MIN and SENSOR_CALIBRATED_MAX from config.hpp are used to map the analog reading to a percentage value.
/// @param sensorNum Index of the sensor in config.hpp(0 to NUM_SENSORS-1)
/// @return Humidity in percent (0 to 99%)
int getHumidity(const int sensorNum) {
  if (getPowerPin(sensorNum) != -1) {
    digitalWrite(getPowerPin(sensorNum), HIGH);
    delay(10);
  }
  int val = (100 - (((float)avgRead(getSensorPin(sensorNum)) - SENSOR_CALIBRATED_MIN) / ((SENSOR_CALIBRATED_MAX - SENSOR_CALIBRATED_MIN) / 100.0)));  //calculate percentage of humidity
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