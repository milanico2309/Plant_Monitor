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

  if(millis() % 10000 < 500) { //every 10 seconds
    View::debugLine(F("Start reading"));
    View::printUpdateScreen();
    Lib::readSensorsAndUpdateMemory();
    View::debugLine(F("Reading done"));
    View::valuesSerialPrint();
    View::valuesSerialPlot();
  }
}
