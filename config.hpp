/**
 * @file config.hpp
 * @brief Compile-time configuration for the Plant Monitor project.
 *
 * Only use this configuration with a compatible source code version.
 * The software is provided "as is" without warranty of any kind.
 * @author Milan Stuhlsatz
 * @date 2025-08-21
 * @version ALPHA
 * @license MIT
 */
#include <stdint.h>
#pragma once

#ifndef SOILMANAGER_CONFIG_H
#define SOILMANAGER_CONFIG_H

#include <Arduino.h>

/*****************************/
/**   Setup your project    **/
/*****************************/

/// General Configuration

/**
 * @def DISP
 * @brief Enable any display output.
 */
#define DISP
/**
 * @def SERIAL_OUT
 * @brief Enable any serial output.
 */
#define SERIAL_OUT
/**
 * @def DEBUG_SERIAL
 * @brief Enable debug output over serial.
 */
#define DEBUG_SERIAL
/**
 * @def SERIAL_PLOT
 * @brief Enable Arduino Serial Plotter-friendly output.
 */
#define SERIAL_PLOT
/**
 * @def DEBUG_DISP
 * @brief Enable debug output on the display.
 */
#define DEBUG_DISP
/**
 * @def SERIAL_LOG
 * @brief Enable human-friendly logs over serial (as opposed to plotter mode).
 */
#define SERIAL_LOG

/**
 * @brief Interval for showing debug messages on display (milliseconds).
 */
constexpr uint16_t T_SHOWDEBUG = 1000;
/**
 * @brief Number of samples to average per sensor read.
 */
constexpr uint8_t AVERAGE_OF = 3;

/*---------------------------------------------------------------*/
/**
 * @def NUM_SENSORS
 * @brief Number of sensors connected.
 */
#define NUM_SENSORS 3
/*---------------------------------------------------------------*/

/// Configuration for each sensor

/**
 * @brief Human-readable identifier for sensor 1 (stored in flash).
 */
#define SENSOR_1_ID F("Monstera")
/**
 * @brief Analog pin for sensor 1.
 */
constexpr uint8_t SENSOR_1_PIN = A0;

/**
 * @brief Human-readable identifier for sensor 2 (stored in flash).
 */
#define SENSOR_2_ID F("Schaefl.")
/**
 * @brief Analog pin for sensor 2.
 */
constexpr uint8_t SENSOR_2_PIN = A1;

/**
 * @brief Human-readable identifier for sensor 3 (stored in flash).
 */
#define SENSOR_3_ID F("Gl.Feder")
/**
 * @brief Analog pin for sensor 3.
 */
constexpr uint8_t SENSOR_3_PIN = A2;

/**
 * @brief Calibrated minimum raw value (sensor immersed in water).
 */
constexpr uint16_t SENSOR_CALIBRATED_MIN = 360;
/**
 * @brief Calibrated maximum raw value (sensor in dry air/soil).
 */
constexpr uint16_t SENSOR_CALIBRATED_MAX = 790;

/************************************************************************/
/** Do NOT edit anything beyond this point (unless you know what you do) **/
/************************************************************************/

// PIN Config.
/**
 * @brief Maximum number of sensors supported by the firmware.
 */
constexpr uint8_t MAX_SENSORS = 3;
static_assert(NUM_SENSORS <= MAX_SENSORS, "Error: NUM_SENSORS exceeds MAX_SENSORS. Please adjust configuration.");

// Advanced Config.
/**
 * @brief Delay in ms between serial prints to avoid overruns.
 */
constexpr uint8_t SERIAL_TRANSMIT_DELAY = 1;
/**
 * @def BAUDRATE
 * @brief Baud rate for the serial connection.
 */
#define BAUDRATE 115200
/**
 * @def ANALOG_REF
 * @brief Analog reference configuration used for analog inputs.
 */
#define ANALOG_REF DEFAULT

#endif