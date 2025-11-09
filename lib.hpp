/**
 * @file lib.hpp
 * @brief Public API for sensor reading and runtime context used by Plant Monitor.
 */
#pragma once
#ifndef PLANTMONITOR_LIB_H
#define PLANTMONITOR_LIB_H

#include "config.hpp"
#include "Arduino.h"

/**
 * @namespace Lib
 * @brief Contains sensor logic and the in-memory context for measured values.
 */
namespace Lib {

/**
 * @brief Holds the latest sensor values as percentages (0–99).
 */
struct SensorContext {
    uint8_t values[MAX_SENSORS];
};

/**
 * @brief Global runtime context containing the latest readings.
 */
extern SensorContext ctx;

/**
 * @brief Returns the sensor name stored in flash for a given index.
 * @param idx Sensor index starting at 0.
 * @return Flash string helper pointer to the configured sensor name.
 */
const __FlashStringHelper* getSensorName(uint8_t idx);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Reads all configured sensors and updates the global context.
 */
void readSensorsAndUpdateMemory();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the global context and configures sensor input pins.
 */
void initCtx();

} // namespace Lib

#endif //PLANTMONITOR_LIB_H
