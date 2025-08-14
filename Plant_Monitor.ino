// Author: Milan Stuhlsatz
// Date: 12.08.2025
// Versiom: ALPHA
// Licens: MIT

#include "lib.h"
#include "view.h"
#include <Arduino.h>

void setup() {

  //initialize output
  initSerial();
  initDisplay();

  //Initialize memory
  initIDArray();
  // initDataArray();

  debugLine(F("starting..."));
}

void loop() {
  // put your main code here, to run repeatedly:
}
