#include "config.h"
#include "types.h"
#include "view.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Arduino.h>

#if DISP >= 1
  Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
#if DEBUG >= 1
  String debug_buffer[10];
#endif  //DEBUG
#endif  //DISP

void message(String message) {
#if SERIAL_OUT >= 1
  Serial.print(message);
#endif  //SERIAL_OUT
}

void messageLine(String message) {
#if SERIAL_OUT >= 1
  Serial.println(message);
#endif  //SERIAL_OUT
}

void v_message(SensorValue sensorValue) {
}

void debugLine(String msg) {
#if DEBUG >= 1
  debugLineSerial(msg);
#endif  //DEBUG
}

void debugLine(const __FlashStringHelper* msg) {
#if DEBUG >= 1
  debugLineSerial(msg);
#endif  //DEBUG
}

void debug(String msg) {
#if DEBUG >= 1
  debugSerial(msg);
#endif  //DEBUG
}

void debug(const __FlashStringHelper* msg) {
#if DEBUG >= 1
  debugSerial(msg);
#endif  //DEBUG
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   SERIAL   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Initialize Serial Communication
void initSerial() {
#if SERIAL_OUT >= 1
  Serial.begin(BAUDRATE);  //open serial port
  debugLine(F("Completed serial setup!"));
#endif  //SERIAL_OUT
}

void debugLineSerial(String msg) {
  Serial.println(msg);
}

void debugLineSerial(const __FlashStringHelper* msg) {
  Serial.println(msg);
}

void debugSerial(String msg) {
  Serial.print(msg);
}

void debugSerial(const __FlashStringHelper* msg) {
  Serial.print(msg);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   DISPLAY   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Initialize the Display
void initDisplay() {
#if DISP >= 1

  debugLine(F("Setup Display..."));
  display.begin(0x3C, true);  // Address on bus, 0x3C default
  display.display();
  delay(1000);
  display.setRotation(1);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  debugLine(F("Test display"));
  display.invertDisplay(true);
  debugLine(F("Set display to white..."));
  display.display();
  delay(1000);
  display.invertDisplay(false);
  debugLine(F("Set display to black..."));
  display.display();
  debugLine(F("Completed Display setup!"));

#endif  //DISP
}