// Author: Milan Stuhlsatz
// Date: 21.08.2025
// Versiom: ALPHA
// Licens: MIT

#include "lib.hpp"
#include "view.hpp"
#include <Arduino.h>

void setup() {

  //initialize output
  View::initSerial();
  View::initDisplay();
  //Initialize memory
  Lib::initCtx();
  Lib::initIDArray();
  View::debugLine(F("starting..."));
}

void loop() {
  View::printMainScreen();
  Lib::readSensorsAndUpdateMemory();
}
