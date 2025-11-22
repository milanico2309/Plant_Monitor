/**
 * @file lib.hpp
 * @brief Public API for sensor reading and runtime context used by Plant Monitor.
 */
#pragma once

#include "config.hpp"
#include "Arduino.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  FUNCTIONS  ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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
const __FlashStringHelper *getSensorName(uint8_t idx);

/**
     * @brief Reads all configured sensors and updates the global context.
     */
void readSensorsAndUpdateMemory();

/**
     * @brief Set the millisecond offset used to compute the effective time.
     * @param offset Signed offset in milliseconds; effectiveTime = millis() + offset
     */
void setTimeOfDayMillisOffset(long offset);

/**
     * @brief Return the effective current time in milliseconds (millis() + offset).
     */
unsigned long getTimeOfDayAsMillis();

/**
     * @brief Request a sensor read to be performed by the main loop (can be set
     * from other modules or an ISR).
     */
void requestSensorRead();

/**
     * @brief Atomically consume and clear the pending sensor-read request.
     * @return true if a request was pending and was cleared, false otherwise.
     */
bool hasSensorReadRequest();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////    SETUP    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
     * @brief Initializes the global context and configures sensor input pins.
     */
void initCtx();
}  // namespace Lib