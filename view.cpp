#include "HardwareSerial.h"
#include "config.h"
#include "types.h"
#include "view.h"
#include "lib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Arduino.h>



#if DISP >= 1
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
int dispScrollOffset = 0;
int sensorIDOffset = 0;
#if DEBUG >= 1
#define DEBUG_BUFFER_LINES 8
#define DEBUG_BUFFER_ROWS 22
char debug_buffer[DEBUG_BUFFER_LINES][DEBUG_BUFFER_ROWS];
int debugBufferLine = 0;
void debugBufferNextLine();
void printDebugBuffer();
void debugLineDisplay(const __FlashStringHelper* msg);
void debugLineSerial(const __FlashStringHelper* msg);
#endif  //DEBUG
#endif  //DISP



void debugLine(const __FlashStringHelper* msg) {
#if DEBUG >= 1
  debugLineSerial(msg);
  debugLineDisplay(msg);
#endif  //DEBUG
}

void printMainScreen() {
  //debugLine(F("Printing Main Screen..."));
  display.clearDisplay();
  display.setTextSize(2);

  int y = dispScrollOffset;
  while(y < 129){
    display.setCursor(0, y);
    display.print(sensorID[sensorIDOffset]);
    sensorIDOffset = (sensorIDOffset + 1) % NUM_SENSORS;
    display.setCursor(106, y);
    int z = random(1,99);
    if(z<10)display.setCursor(118, y);
    else display.setCursor(106, y);
    display.print(z);
    y+=16;
  }
  dispScrollOffset = (dispScrollOffset-1)%-16;
  if(dispScrollOffset == 0){sensorIDOffset = (sensorIDOffset + 1) % NUM_SENSORS;}
  display.fillRect(0, 0, 128, 10, SH110X_BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.invertDisplay(false);

  millisOffset = 11640000;
  unsigned long actualSeconds = (millis()+millisOffset)/1000;
  unsigned long daySeconds = actualSeconds % 86400;
  int timeSeconds = daySeconds % 60;
  int timeMinutes = (daySeconds / 60) % 60;
  int timeHours = (daySeconds / 3600) % 24;
  char buf[8];;;
  sprintf(buf,"%02d:%02d:%02d",timeHours,timeMinutes,timeSeconds);
  display.println(buf);

  display.drawFastHLine(0, 9, 128, SH110X_WHITE);

  display.display();

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   SERIAL   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Initialize Serial Communication
void initSerial() {
#if SERIAL_OUT >= 1
  Serial.begin(BAUDRATE);  //open serial port
  Serial.println(F("Completed serial setup!"));
#endif  //SERIAL_OUT
}

void debugLineSerial(const __FlashStringHelper* msg) {
  Serial.println(msg);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   DISPLAY   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Initialize the Display
void initDisplay() {
#if DISP >= 1

  debugLineSerial(F("Setup Display..."));
  display.begin(0x3C, true);  // Address on bus, 0x3C default
  display.cp437(true);
  display.setTextWrap(false);
  display.display();
  delay(1000);
  display.setRotation(0);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.display();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  debugLineSerial(F("Test display"));
  display.invertDisplay(true);
  debugLineSerial(F("Set display to white..."));
  display.display();
  delay(500);
  display.invertDisplay(false);
  debugLineSerial(F("Set display to black..."));
  display.display();
  delay(200);
  debugLine(F("Completed Display setup!"));

#endif  //DISP
}

void debugLineDisplay(const __FlashStringHelper* msg) {
  debugBufferNextLine();
  strncpy_P(debug_buffer[debugBufferLine], (PGM_P)msg, DEBUG_BUFFER_ROWS - 1);  // Kopie von Flash in SRAM
  debug_buffer[debugBufferLine][DEBUG_BUFFER_ROWS - 1] = '\0';                  // Sicherheit: Nullterminierung
  printDebugBuffer();
}

void debugBufferNextLine() {
  debugBufferLine = (debugBufferLine + 1) % DEBUG_BUFFER_LINES;  //after max rollover to 0
}

void printDebugBuffer() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.invertDisplay(true);
  for (int i = 1; i <= DEBUG_BUFFER_LINES; i++) {
    display.println(debug_buffer[(debugBufferLine + i) % DEBUG_BUFFER_LINES]);  //We start at debugLine+1 as its the least recently written line and work our way through the ringbuffer.
  }
  display.display();
  delay(50);
}
