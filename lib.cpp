#include "lib.h"
#include "config.h"
#include "Arduino.h"
#include "view.h"

namespace Lib {

int sensorData[NUM_SENSORS];  // Contains all relevant data about each sensor in this order: VALUE, INPUT PIN, CALIBRATED MINIMUM, CALIBRATED MAXIMUM. (See enum in lib.h)
const __FlashStringHelper* sensorID[NUM_SENSORS];

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Reads raw sensor data and generates an average over a defined number of meassurments
int avgRead(int addr) {
  int values = 0;  //stores values for average calculation
  for (int i = 0; i < AVERAGE_OF; i++) values += analogRead(addr);            //read input value from sensor
  return (float)values / (float)AVERAGE_OF;  //calculate average
}

//Returns humidity as a percent value, based on a scale between the calibrated MIN/MAX of the sensor.
int getHumidity(int x) {
  return (100 - (((float)avgRead(14 + x) - SENSOR_CALIBRATED_MIN) / ((SENSOR_CALIBRATED_MAX - SENSOR_CALIBRATED_MIN) / 100.0)));  //calculate percentage of humidity (For Constants see Enum in const.h)
}

//Read all sensors and write results to memory
void readSensorsAndUpdateMemory() {

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorData[i] = getHumidity(i);
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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