// Author: Milan Stuhlsatz
// Date: 21.08.2025
// Versiom: ALPHA
// Licens: MIT

#include "lib.hpp"
#include "view.hpp"
#include <Arduino.h>

extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0) {
    // Heap wurde noch nicht benutzt
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}

void setup() {

  //initialize output
  View::initSerial();
  Serial.print(F("Free Memory: "));
  Serial.println(freeMemory());
  View::initDisplay();
  //Initialize memory
  Lib::initCtx();
  Lib::initIDArray();
  Serial.print(F("Free Memory: "));
  Serial.println(freeMemory());
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
    Serial.print(F("Free Memory: "));
    Serial.println(freeMemory());
  }

}
