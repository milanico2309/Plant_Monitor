/**
 * @file view.hpp
 * @brief Public UI/IO interface for serial and display output.
 */
#pragma once
#ifndef VIEW_H
#define VIEW_H

#include "Arduino.h"

/**
 * @defgroup view_ui View / UI
 * @brief Serial logs/plots and OLED rendering utilities.
 */

/**
 * @namespace View
 * @brief Functions for user-visible output (serial logs/plots and OLED display).
 * @ingroup view_ui
 */
namespace View {

/**
 * @brief Append a debug line to the on-device debug buffer and redraw it.
 * @param msg Flash-stored message to show.
 */
void debugLineDisplay(const __FlashStringHelper* msg);

void debugLineDisplay(long msg);

template<typename T>
void debugLineSerial(T msg) {
#ifdef DEBUG_SERIAL
  Serial.println(msg);
#endif
}

template<typename T>
void debugSerial(T msg) {
#ifdef DEBUG_SERIAL
  Serial.print(msg);
#endif
}

template<typename T>
void messageLineSerial(T msg) {
#ifdef SERIAL_OUT
  Serial.println(msg);
#endif
}

template<typename T>
void messageSerial(T msg) {
#ifdef SERIAL_OUT
  Serial.print(msg);
#endif
}

template<typename T>
inline void debugLine(T msg) {
  debugLineSerial(msg);
  debugLineDisplay(msg);
}

template<typename T>
inline void debug(T msg) {
  debugSerial(msg);
  debugDisplay(msg);
}

template<typename T>
inline void messageLine(T msg) {
  messageLineSerial(msg);
  debugLineDisplay(msg);
}

template<typename T>
inline void message(T msg) {
  messageSerial(msg);
  debugLineDisplay(msg);
}

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

/**
   * @brief Enable or disable the OLED display output at runtime.
   * When disabling, the display is cleared once.
   */
void setDisplayEnabled(bool enabled);

/**
   * @brief Query whether the display output is currently enabled.
   */
bool isDisplayEnabled();

/**
   * @brief Set OLED display contrast at runtime (0–255). Values will be clamped.
   */
void setDisplayContrast(uint8_t value);
}  // namespace View
#endif  //VIEW_H
