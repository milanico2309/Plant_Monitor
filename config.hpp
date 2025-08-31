#pragma once
// Author: Milan Stuhlsatz
// Date: 21.08.2025
// Versiom: ALPHA
// Licens: MIT

// Compatible: ALPHA
// Make shure to ONLY use this Configuration-File with source code of a compatible Version number.
// This software comes as is and without any warranty. Use under your own risk only.

#ifndef SOILMANAGER_CONFIG_H
#define SOILMANAGER_CONFIG_H

#include <Arduino.h>

/*****************************/
/**   Setup your project    **/
/*****************************/



  //General Configuration

  #define DISP                  //Activates Display Output
  #define SERIAL_OUT            //Activates Serial Communication
  #define DEBUG_SERIAL          //Activates Debug outtput via Serial
  //#define SERIAL_PLOT           //Activates Serial Plotter output
  #define DEBUG_DISP            //Activates Debug output via Display

  const uint16_t T_SHOWDEBUG = 1000;
  const uint8_t AVERAGE_OF  = 5;     //how many readings will be averaged per measurement.

  /*---------------------------------------------------------------*/
  #define NUM_SENSORS        3         //Amount of sensors connected.
  /*---------------------------------------------------------------*/

//Configuration for each loop

  //Sensor 1
  #define SENSOR_1_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_1_PIN =        A0;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_1_PPIN =       11;           //Pin that powers the sensor (if applicable). Set to -1 if not used.
  //Sensor 2
  #define SENSOR_2_ID             F("Schaefl.")  //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_2_PIN =        A1;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_2_PPIN =       12;           //Pin that powers the sensor (if applicable). Set to -1 if not used.

  //Sensor 3
  #define SENSOR_3_ID             F("Gl.Feder") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_3_PIN =        A2;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_3_PPIN =       13;           //Pin that powers the sensor (if applicable). Set to -1 if not used.

  //Sensor 4
  #define SENSOR_4_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_4_PIN =        A3;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_4_PPIN =       14;           //Pin that powers the sensor (if applicable). Set to -1 if not used.  

  //Sensor 5
  #define SENSOR_5_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_5_PIN =        A6;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_5_PPIN =       15;           //Pin that powers the sensor (if applicable). Set to -1 if not used.

  //Sensor 6
  #define SENSOR_6_ID             F("Monstera") //A Name by which this sensor is identified (e.g. Name of the corresponding plant)
  const uint8_t SENSOR_6_PIN =        A7;           //Analog Pin to which the sensor is connected.
  const uint8_t SENSOR_6_PPIN =       16;           //Pin that powers the sensor (if applicable). Set to -1 if not used.

  //Calibration values for the sensors.
  //These values are used to map the analog reading to a percentage value.
  //You need to measure these values for your specific sensor and setup.
  
  const uint16_t SENSOR_CALIBRATED_MIN = 360; //meassured analog value if sensor is in water.
  const uint16_t SENSOR_CALIBRATED_MAX = 790; //meassured analog value if sensor is dry.



  

/************************************************************************/
/** Do NOT edit anything beyond this point (unless u know what you do) **/
/************************************************************************/

//PIN Config.
  const uint8_t MAX_SENSORS = 6;      //Maximum amount of sensors supported by this software.
  static_assert(NUM_SENSORS <= MAX_SENSORS, "Error: NUM_SENSORS exceeds MAX_SENSORS. Please adjust configuration.");

//Advanced Config.
  const uint8_t SERIAL_TRANSMIT_DELAY = 5; //Delay in ms between sending messages via Serial. Increase this if you experience problems with your serial connection.
  #define BAUDRATE          57600    //Boudrate for serial connection, match this to your terminal. If you don't know what this means, don't change it.
  #define ANALOG_REF        DEFAULT  //Configures the reference voltage used for analog input.
#endif