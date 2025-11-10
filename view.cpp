/**
 * @file view.cpp
 * @brief Implementation of serial and OLED display rendering for Plant Monitor.
 */
#include <Arduino.h>
#include "config.hpp"
#include "view.hpp"
#include "lib.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <string.h>

namespace View {

/**
 * @brief Runtime switch to enable/disable display rendering.
 */
static bool displayEnabled = true;

/**
 * @brief Format `millis()` (+ offset) into HH:MM:SS.
 * @param buf Buffer of at least 9 chars to receive a null-terminated string.
 * @param flashDots If true, the separators blink once per second.
 */
void formatMillisTime(char* buf, bool flashDots = false);

#ifdef DISP
/** OLED driver instance for a 128x64 SH1106 display. */
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
/** Vertical pixel offset used for marquee-style scrolling. */
int8_t dispScrollOffset = 0;
/** Index of the next sensor name/value to render at the top line. */
uint8_t sensorIDOffset = 0;
/** Draw the current time header bar at the top of the screen. */
static void drawTime();
/** Timestamp of the last debug message shown (for auto-hide). */
unsigned long lastDebug = 0;

#endif  //DISP
#ifdef DEBUG_DISP
#define DEBUG_BUFFER_LINES 8
#define DEBUG_BUFFER_ROWS 22
char debug_buffer[DEBUG_BUFFER_LINES][DEBUG_BUFFER_ROWS];
int debugBufferLine = 0;
static void debugBufferNextLine();
static void printDebugBuffer();
#endif  //DEBUG_DISP

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   SERIAL   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize the Serial interface (when @ref SERIAL_OUT is enabled).
 *
 * Opens @ref Serial at @ref BAUDRATE and prints a confirmation debug line
 * when @ref DEBUG_SERIAL is defined.
 */
void initSerial() {
#ifdef SERIAL_OUT
  Serial.begin(BAUDRATE);  // open serial port
  debugLineSerial(F("Completed serial setup!"));
#endif  // SERIAL_OUT
}


void valuesSerialPrint() {
#if defined(SERIAL_LOG) && defined(SERIAL_OUT)
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    messageSerial(Lib::getSensorName(i));
    messageSerial(F(": "));
    messageSerial(Lib::ctx.values[i]);
    messageSerial(' ');
  }
  messageLineSerial(F(""));
  // non-blocking: serial templates avoid blocking; no delay here
#endif  //SERIAL_LOG
}

void valuesSerialPlot() {
#ifdef SERIAL_PLOT
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    messageSerial(Lib::ctx.values[i]);
    messageSerial(' ');
  }
  messageLineSerial(F(""));
  // non-blocking: serial templates avoid blocking; no delay here
#endif  //SERIAL_PLOT
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   DISPLAY   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Initialize the Display
void initDisplay() {
#ifdef DISP
  debugLineSerial(F("Setup Display..."));
  if (!display.begin(0x3C, false)) {  // reset = false
    debugLineSerial(F("Display not found!"));
    for (;;)
      ;  // stop here
  }
  display.cp437(true);
  display.setTextWrap(false);
  display.setContrast(255);
  display.display();
  delay(500);
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
  display.setContrast(DISP_CONTRAST);
  debugLine(F("Completed Display setup!"));
  // Ensure runtime flag starts enabled
  displayEnabled = true;
#endif  //DISP
}



void debugLineDisplay(const __FlashStringHelper* msg) {
#if defined(DEBUG_DISP) && defined(DISP)
  if (!displayEnabled) return;  // respect runtime display switch
  lastDebug = millis();
  debugBufferNextLine();
  strncpy_P(debug_buffer[debugBufferLine], (PGM_P)msg, DEBUG_BUFFER_ROWS - 1);  // Kopie von Flash in SRAM
  debug_buffer[debugBufferLine][DEBUG_BUFFER_ROWS - 1] = '\0';                  // Sicherheit: Nullterminierung
  printDebugBuffer();
#endif  //DEBUG_DISP
}

void debugLineDisplay(long value) {
#if defined(DEBUG_DISP) && defined(DISP)
  if (!displayEnabled) return;
  lastDebug = millis();
  debugBufferNextLine();
  snprintf(debug_buffer[debugBufferLine], DEBUG_BUFFER_ROWS, "%ld", value);  // long → String
  printDebugBuffer();
#endif  //DEBUG_DISP
}

void debugBufferNextLine() {
#if defined(DEBUG_DISP) && defined(DISP)
  debugBufferLine = (debugBufferLine + 1) & (DEBUG_BUFFER_LINES - 1);  //after max rollover to 0
#endif                                                                 //DEBUG_DISP
}

void printDebugBuffer() {
#if defined(DEBUG_DISP) && defined(DISP)
  display.clearDisplay();
  display.setCursor(0, 0);
  display.invertDisplay(true);
  for (uint8_t i = 1; i <= DEBUG_BUFFER_LINES; i++) {
    display.println(debug_buffer[(debugBufferLine + i) % DEBUG_BUFFER_LINES]);  //We start at debugLine+1 as its the least recently written line and work our way through the ringbuffer.
  }
  display.display();
  delay(50);
#endif  //DEBUG_DISP
}

void printMainScreen() {
#if defined(DISP)
  if (!displayEnabled) return;
#if defined(DEBUG_DISP)
  if (lastDebug + T_SHOWDEBUG < millis()) {
#endif  //DEBUG_DISP
    display.clearDisplay();
    display.setTextSize(2);
    display.invertDisplay(false);

    int16_t y = dispScrollOffset;
    while (y < 129) {
      display.setCursor(0, y);
      display.print(Lib::getSensorName(sensorIDOffset));
      display.setCursor(106, y);
      display.print(Lib::ctx.values[sensorIDOffset]);
      sensorIDOffset = (sensorIDOffset + 1) % NUM_SENSORS;
      y += 16;
    }
    dispScrollOffset = (dispScrollOffset - 1) % -16;
    if (dispScrollOffset == 0) { sensorIDOffset = (sensorIDOffset + 1) % NUM_SENSORS; }

    drawTime();
    display.display();

#ifdef DEBUG_DISP
  }
#endif  //DEBUG_DISP
#endif  //DISP
}

void printUpdateScreen() {
#if defined(DISP)
  if (!displayEnabled) return;
  display.clearDisplay();
  display.setTextSize(2);
  display.invertDisplay(false);

  display.setCursor(19, 21);
  display.println(F("Updating"));
  display.setCursor(8, 37);
  display.println(F("sensors..."));

  drawTime();

  display.display();
#endif  //DISP
}

static void drawTime() {
#if defined(DISP)
  display.fillRect(0, 0, 128, 11, SH110X_BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  char buf[9];
  formatMillisTime(buf, true);
  display.println(buf);
  display.drawFastHLine(0, 9, 128, SH110X_WHITE);
#endif  //DISP
}

void formatMillisTime(char* buf, bool flashDots) {
  // Calculate hours, minutes, seconds based on effective millis provided by Lib
  uint32_t totalSeconds = (Lib::getEffectiveMillis()) / 1000;
  uint8_t hours = (totalSeconds / 3600) % 24;
  uint8_t minutes = (totalSeconds / 60) % 60;
  uint8_t seconds = totalSeconds % 60;

  // Format time string with optional flashing colons
  char sep = ':';
  if (flashDots && (seconds % 2 != 0)) sep = ' ';

  // Fill buffer
  buf[0] = '0' + (hours / 10);
  buf[1] = '0' + (hours % 10);
  buf[2] = sep;
  buf[3] = '0' + (minutes / 10);
  buf[4] = '0' + (minutes % 10);
  buf[5] = sep;
  buf[6] = '0' + (seconds / 10);
  buf[7] = '0' + (seconds % 10);
  buf[8] = '\0';
}

bool isDisplayEnabled() {
  return displayEnabled;
}

void setDisplayEnabled(bool enabled) {
  if (displayEnabled == enabled) return;
  displayEnabled = enabled;
#ifdef DISP
  if (!enabled) {
    // Ensure the physical display is blanked when disabling output
    display.clearDisplay();
    display.display();
  }
#endif
}

void setDisplayContrast(uint8_t value) {
#ifdef DISP
  // Clamp to valid range for SH1106
  if (value > 255) value = 255;
  display.setContrast(value);
#endif
#ifdef SERIAL_OUT
#ifdef DEBUG_SERIAL
  // Use templated helper; it will skip if TX buffer is full
  Serial.print(F("Contrast set to "));
  Serial.println((int)value);
  // no delay
#endif
#endif
}

}  // namespace View
