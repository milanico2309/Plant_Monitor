// Author: Milan Stuhlsatz
// Date: 21.08.2025
// Versiom: ALPHA
// Licens: MIT

#include "lib.h"
#include "view.h"
#include <Arduino.h>

void setup() {

  //initialize output
  View::initSerial();
  View::initDisplay();
  //Initialize memory
  Lib::initIDArray();
  View::debugLine(F("starting..."));
}

void loop() {
  View::printMainScreen();
  Lib::readSensorsAndUpdateMemory();
}
