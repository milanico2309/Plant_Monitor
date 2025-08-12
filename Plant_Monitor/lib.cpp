/************************************************************************/
/** Do NOT edit anything beyond this point (unless u know what you do) **/
/************************************************************************/

#include "lib.h"
#include "const.h"
#include "types.h"

int sensorData[NUM_SENSORS][3]; // Contains all relevant data about each sensor in this order: VALUE, INPUT PIN, CALIBRATED MINIMUM, CALIBRATED MAXIMUM. (See enum in lib.h)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Read all sensors and write results to memory
void readSensorsAndUpdateMemory() {

  debugLine("Reading all Sensors");

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorData[i][VALUE] = getHumidity(i);
  }
}

//Returns humidity as a percent value, based on a scale between the calibrated MIN/MAX of the sensor.
int getHumidity(int x) {

  debug("Reading Humidity of:");
  debugLine(x);

  return ( 100 - ( ( (float) avgRead(sensorData[x][PIN]) - sensorData[x][CALIBRATED_MIN] ) / ( ( sensorData[x][CALIBRATED_MAX] - sensorData[x][CALIBRATED_MIN] ) / 100.0 ) ) ); //calculate percentage of humidity (For Constants see Enum in const.h)
}

//Reads raw sensor data and generates an average over a defined number of meassurments
int avgRead(int addr) {

  int values = 0; //stores values for average calculation
  for ( int i = 0; i < AVERAGE_OF; i++) {
    values += analogRead(addr); //read input value from sensor
    delay( T_AVERAGE * T_AVERAGE_UNIT / AVERAGE_OF ); //wait between measurments
  }
  return (float)values / (float)AVERAGE_OF; //calculate average
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Write human friendly names of sensors to memory.
void initIDArray() {
  debugLine("Init IDs");
  #if NUM_SENSORS >= 1
  memcpy(sensorID[0], SENSOR_1_ID, sizeof(SENSOR_1_ID));
  #endif
  #if NUM_SENSORS >= 2
  memcpy(sensorID[1], SENSOR_2_ID, sizeof(SENSOR_2_ID));
  #endif
  #if NUM_SENSORS >= 3
  memcpy(sensorID[2], SENSOR_3_ID, sizeof(SENSOR_3_ID));
  #endif
  #if NUM_SENSORS >= 4
  memcpy(sensorID[3], SENSOR_4_ID, sizeof(SENSOR_4_ID));
  #endif
  #if NUM_SENSORS >= 5
  memcpy(sensorID[4], SENSOR_5_ID, sizeof(SENSOR_5_ID));
  #endif
  #if NUM_SENSORS >= 6
  memcpy(sensorID[5], SENSOR_6_ID, sizeof(SENSOR_6_ID));
  #endif
}

//initializes loopdate
void initDataArray() {

  #if NUM_SENSORS >= 1

  debug("Init Sensor 1...");

  sensorData[0][PIN] = SENSOR_1_PIN;
  sensorData[0][CALIBRATED_MIN] = SENSOR_1_CALIBRATED_MIN;
  sensorData[0][CALIBRATED_MAX] = SENSOR_1_CALIBRATED_MAX;

  debugLine("done!");

  #endif
  #if NUM_SENSORS >= 2

  debug("Init Sensor 2...");

  sensorData[1][PIN] = SENSOR_2_PIN;
  sensorData[1][CALIBRATED_MIN] = SENSOR_2_CALIBRATED_MIN;
  sensorData[1][CALIBRATED_MAX] = SENSOR_2_CALIBRATED_MAX;

  debugLine("done!");

  #endif
  #if NUM_SENSORS >= 3

  debug("Init Sensor 3...");

  sensorData[2][PIN] = SENSOR_3_PIN;
  sensorData[2][CALIBRATED_MIN] = SENSOR_3_CALIBRATED_MIN;
  sensorData[2][CALIBRATED_MAX] = SENSOR_3_CALIBRATED_MAX;

  debugLine("done!");

  #endif
  #if NUM_SENSORS >= 4

  debug("Init Sensor 4...");

  sensorData[3][PIN] = SENSOR_4_PIN;
  sensorData[3][CALIBRATED_MIN] = SENSOR_4_CALIBRATED_MIN;
  sensorData[3][CALIBRATED_MAX] = SENSOR_4_CALIBRATED_MAX;

  debugLine("done!");

  #endif
  #if NUM_SENSORS >= 5

  debug("Init Sensor 5...");

  sensorData[4][PIN] = SENSOR_5_PIN;
  sensorData[4][CALIBRATED_MIN] = SENSOR_5_CALIBRATED_MIN;
  sensorData[4][CALIBRATED_MAX] = SENSOR_5_CALIBRATED_MAX;

  debugLine("done!");

  #endif
  #if NUM_SENSORS >= 6

  debug("Init Sensor 6...");

  sensorData[5][PIN] = SENSOR_6_PIN;
  sensorData[5][CALIBRATED_MIN] = SENSOR_6_CALIBRATED_MIN;
  sensorData[5][CALIBRATED_MAX] = SENSOR_6_CALIBRATED_MAX;

  debugLine("done!");

  #endif

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   OUTPUT   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Outputs last read sensor data via activated channels
  void printValues(){
    SensorValue sensorValue;
    for (int i = 0; i < NUM_SENSORS; i++) {
      sensorValue.id = i;
      sensorValue.friendlyName = sensorID[i]
      sensorValue.value = sensorData[i][VALUE]
      v_message(sensorValue)
    }
  }