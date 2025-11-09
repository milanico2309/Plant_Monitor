/**
 * @file view.hpp
 * @brief Public UI/IO interface for serial and display output.
 */
#pragma once
#ifndef VIEW_H
#define VIEW_H

#include "Arduino.h"

/**
 * @namespace View
 * @brief Functions for user-visible output (serial logs/plots and OLED display).
 */
namespace View {

/**
 * @brief Prints a single debug line to the enabled outputs.
 * @param msg Flash-stored string to print.
 */
void debugLine(const __FlashStringHelper* msg);

/**
 * @brief Applies an offset to the shown time (for the on-screen clock).
 * @param offset Milliseconds to add to `millis()` when rendering time.
 */
void setMillisOffset(long offset);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   SERIAL   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize serial communications according to @ref BAUDRATE.
 */
void initSerial();

/**
 * @brief Print human-friendly values over serial (requires @ref SERIAL_LOG).
 */
void valuesSerialPrint();

/**
 * @brief Print values formatted for the Arduino Serial Plotter (requires @ref SERIAL_PLOT).
 */
void valuesSerialPlot();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   DISPLAY   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize the OLED display (only when @ref DISP is enabled).
 */
void initDisplay();
/**
 * @brief Render the main screen showing sensor values and status.
 */
void printMainScreen();
/**
 * @brief Render a temporary update screen while sensors are being read.
 */
void printUpdateScreen();

} // namespace View

#endif  //VIEW_H

