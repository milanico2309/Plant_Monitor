/**
 * @file Plant_Monitor.ino
 * @brief Arduino entry point for the Plant Monitor project. Handles setup and main loop.
 * @author Milan Stuhlsatz
 * @date 2025-08-21
 * @version ALPHA
 * @license MIT
 */

#include "lib.hpp"
#include "view.hpp"
#include <Arduino.h>

/**
 * @brief Read all sensors once and update the in-memory context.
 *
 * Also prints short debug messages to the selected outputs and shows the
 * update screen on the display (if enabled).
 */
void readSensors() {
  View::debugLine(F("Start reading"));
  View::printUpdateScreen();
  Lib::readSensorsAndUpdateMemory();
  View::debugLine(F("Reading done"));
}

/**
 * @brief Arduino setup routine.
 *
 * Initializes serial output and display (depending on configuration) and
 * prepares the sensor context and input pins.
 */
void setup() {
  //initialize output
  View::initSerial();
  View::initDisplay();
  //Initialize memory
  Lib::initCtx();
  View::debugLine(F("starting..."));
}

/**
 * @brief Arduino main loop.
 *
 * Continuously renders the main screen and, every ~10 seconds, triggers a
 * sensor read and sends the values over the configured serial outputs.
 */
void loop() {
  View::printMainScreen();
  if(millis() % 10000 < 500) { //every 10 seconds
    readSensors();
    View::valuesSerialPrint();
    View::valuesSerialPlot();
  }
}