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
 * @def SERIAL_IN
 * @brief Enable serial command input (SerialController). If undefined, command parsing is removed at compile-time.
 */
#define SERIAL_IN
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

#define WIRE_HAS_TIMEOUT


/**
 * @brief Interval for showing debug messages on display (milliseconds).
 */
constexpr uint16_t T_SHOWDEBUG = 2000;
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
static_assert(NUM_SENSORS > 0, "NUM_SENSORS must be greater than 0");
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
#define SENSOR_2_ID F("Schaeflerer")
/**
 * @brief Analog pin for sensor 2.
 */
constexpr uint8_t SENSOR_2_PIN = A1;

/**
 * @brief Human-readable identifier for sensor 3 (stored in flash).
 */
#define SENSOR_3_ID F("Gl. Feder")
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
 * @def BAUDRATE
 * @brief Baud rate for the serial connection.
 */
#define BAUDRATE 115200
/**
 * @def ANALOG_REF
 * @brief Analog reference configuration used for analog inputs.
 */
#define ANALOG_REF DEFAULT

#define DISP_CONTRAST 0

/**
 * @brief Interval in seconds for automatic sensor reads when using Timer1.
 *
 * Note: On an ATmega328P (16 MHz) with Timer1 prescaler 1024 the maximum
 * single-shot interval representable with OCR1A (16-bit) is ~4.19 seconds.
 * Set this to a value <= 4 to avoid needing a software counter inside the
 * ISR. If you need a longer interval, use a smaller base interval or the
 * existing millis()-based approach.
 */
constexpr uint8_t READ_INTERVAL_SECONDS = 1;

/**
 * @brief Desired overall sensor-read interval in seconds. The timer will
 * fire every READ_INTERVAL_SECONDS and the ISR will count up to reach this
 * target. Set to any positive value; actual interval will be
 * multiplier * READ_INTERVAL_SECONDS where multiplier = ceil(READ_TARGET_SECONDS / READ_INTERVAL_SECONDS).
 */
constexpr uint8_t READ_TARGET_SECONDS = 10;

// Validate that the base interval is representable for Timer1 with prescaler 1024
static_assert(READ_INTERVAL_SECONDS >= 1 && (unsigned long)(F_CPU / 1024UL) * (unsigned long)READ_INTERVAL_SECONDS - 1UL <= 0xFFFFUL, "READ_INTERVAL_SECONDS too large for Timer1 with prescaler 1024 on this F_CPU; choose a smaller value or use a smaller prescaler.");

// Ensure the computed multiplier will fit in a uint8_t
static_assert(((READ_TARGET_SECONDS + READ_INTERVAL_SECONDS - 1) / READ_INTERVAL_SECONDS) <= 0xFF, "READ_TARGET_SECONDS/READ_INTERVAL_SECONDS multiplier too large to store in uint8_t.");

#endif