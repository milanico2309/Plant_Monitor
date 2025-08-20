// Author: Milan Stuhlsatz
// Date: 12.08.2025
// Versiom: ALPHA
// Licens: MIT

// Compatible: ALPHA
// Make shure to ONLY use this Configuration-File with source code of a compatible Version number.
// This software comes as is and without any warranty. Use under your own risk only.


#ifndef SOILMANAGER_CONFIG_H
#define SOILMANAGER_CONFIG_H

#include "const.h"

/*****************************/
/**   Setup your project    **/
/*****************************/

  /*---------------------------------------------------------------*/
  #define NUM_SENSORS        3         //Amount of sensors connected.
  /*---------------------------------------------------------------*/

  //General Configuration

  #define DISP            1       //Activates Display Output (0..1)
  #define SERIAL_OUT      1       //Activates Serial Communication (0..1)
  #define DEBUG           1       //Activates Debug Mode (0..2)
  
  #define T_COOLDOWN      5     //time to wait between measurments(seconds)
  #define T_AVERAGE       1     //time over wich values will averaged (seconds)
  #define T_CALIBRATION   5     //time over wich target value will be calibrated
  #define T_AVERAGE_UNIT  F_SECONDS

  #define AVERAGE_OF    5     //how many readings will be averaged per measurement.

//Configuration for each loop

  //Sensor 1
  #define SENSOR_1_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_1_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_1_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_1_PIN            14            //To which Pin the Sensor is connected

  //Sensor 2
  #define SENSOR_2_ID             F("unknown") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_2_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_2_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_2_PIN            14            //To which Pin the Sensor is connected

  //Sensor 3
  #define SENSOR_3_ID             F("Gl.Feder") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_3_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_3_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_3_PIN            14            //To which Pin the Sensor is connected

  //Sensor 4
  #define SENSOR_4_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_4_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_4_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_4_PIN            14            //To which Pin the Sensor is connected

  //Sensor 5
  #define SENSOR_5_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_5_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_5_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_5_PIN            14            //To which Pin the Sensor is connected

  //Sensor 6
  #define SENSOR_6_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  #define SENSOR_6_CALIBRATED_MIN 350           //meassured analog value if sensor is in water.
  #define SENSOR_6_CALIBRATED_MAX 800           //meassured analog value if sensor is dry.
  #define SENSOR_6_PIN            14            //To which Pin the Sensor is connected



  

/************************************************************************/
/** Do NOT edit anything beyond this point (unless u know what you do) **/
/************************************************************************/

//PIN Config.

//Advanced Config.
  #define BAUDRATE          9600    //Boudrate for serial connection, match this to your terminal. If you don't know what this means, don't change it.
  #define ANALOG_REF        DEFAULT //Configures the reference voltage used for analog input.
#endif