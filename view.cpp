/**
 * @file view.cpp
 * @brief Implementation of serial and OLED display rendering for Plant Monitor.
 */
#include <Arduino.h>
#include "config.hpp"
#include "view.hpp"
#include "lib.hpp"
#include <U8g2lib.h>
#include <Wire.h>

namespace View {

   static_assert(NUM_SENSORS > 0, "NUM_SENSORS must be greater than 0");

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
U8G2_SH1106_128X64_NONAME_2_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
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
#define DEBUG_BUFFER_LINES 6
#define DEBUG_BUFFER_ROWS 21
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
#endif  //SERIAL_LOG
}

void valuesSerialPlot() {
#ifdef SERIAL_PLOT
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    messageSerial(Lib::ctx.values[i]);
    messageSerial(' ');
  }
  messageLineSerial(F(""));
#endif  //SERIAL_PLOT
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////   DISPLAY   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize I2C communication with 400kHz clock speed.
 * 
 * Sets up the Wire (I2C) interface with a clock frequency of 400kHz
 * for fast communication with the OLED display.
 */
void initI2C() {
  Wire.begin();
  Wire.setClock(400000);
  //Wire.setWireTimeout(1000, true);
}

/**
 * @brief Initialize the OLED display for Plant Monitor (when @ref DISP is enabled).
 * 
 * This function:
 * 1. Sets up I2C communication with the display
 * 2. Initializes the display driver
 * 3. Enables UTF-8 text output
 * 4. Shows a brief startup message
 * 5. Sets the display contrast to @ref DISP_CONTRAST
 * 
 * The function is guarded by the @ref DISP compile-time flag and will do nothing
 * if the display support is not enabled.
 */
void initDisplay() {
#ifdef DISP
  debugLineSerial(F("Setup Display..."));
  initI2C();
  display.begin();
  //display.enableUTF8Print();
  display.firstPage();
  do {
    display.setFont(u8g2_font_6x12_tf);
    display.setCursor(0, 12);
    display.print(F("Plant Monitor"));
  } while (display.nextPage());

  // Apply desired contrast if supported by controller
  display.setContrast(DISP_CONTRAST);

  debugLine(F("Completed Display setup!"));
  // Ensure runtime flag starts enabled
  displayEnabled = true;
#endif  //DISP
}

  
/**
 * @brief Display a debug message from flash memory on the OLED display.
 * 
 * When both DEBUG_DISP and DISP are enabled, this function adds the given message
 * to a circular debug message buffer and updates the display to show recent messages.
 * Messages are rendered in a scrolling fashion with the most recent at the bottom.
 * 
 * @param msg Flash string pointer to the debug message to display
 */
void debugLineDisplay(const __FlashStringHelper *msg) {
#if defined(DEBUG_DISP) && defined(DISP)
  if (!displayEnabled) return; // respect runtime display switch
  lastDebug = millis();
  debugBufferNextLine();
  strncpy_P(debug_buffer[debugBufferLine], (PGM_P) msg, DEBUG_BUFFER_ROWS - 1); // Kopie von Flash in SRAM
  debug_buffer[debugBufferLine][DEBUG_BUFFER_ROWS - 1] = '\0'; // Sicherheit: Nullterminierung
  printDebugBuffer();
#endif  //DEBUG_DISP
}

/**
 * @brief Display a numeric debug value on the OLED display.
 * 
 * When both DEBUG_DISP and DISP are enabled, this function converts the given
 * numeric value to a string, adds it to the circular debug message buffer,
 * and updates the display to show recent messages.
 * 
 * @param value Long integer value to display as a debug message
 */
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
  debugBufferLine = (debugBufferLine + 1) % DEBUG_BUFFER_LINES;
#endif                                                                 //DEBUG_DISP
}

void printDebugBuffer() {
#if defined(DEBUG_DISP) && defined(DISP)
  display.firstPage();
  do {
    display.setDrawColor(1);
    display.drawBox(0, 0, 128, 64);
    display.setDrawColor(0);
    display.setFont(u8g2_font_profont11_mr);
    for (uint8_t i = 1; i <= DEBUG_BUFFER_LINES; i++) {
      uint8_t idx = (debugBufferLine + i) % DEBUG_BUFFER_LINES;
      int16_t y = i * 10; // 10 px line height for 6*11 font
      display.setCursor(0, y);
      display.print(debug_buffer[idx]);
    }
  } while (display.nextPage());
#endif  //DEBUG_DISP
}

void printMainScreen() {
#if defined(DISP)
  if (!displayEnabled) return;
#if defined(DEBUG_DISP)
  if (lastDebug + T_SHOWDEBUG > millis()) return;
#endif  //DEBUG_DISP
  display.firstPage();
  do {
    display.setFont(u8g2_font_profont17_mr);
    display.setDrawColor(1);
    int16_t y = dispScrollOffset;
    uint8_t localSensorIdx = sensorIDOffset;
    while (y < 129) {
      display.setCursor(0, y);
      display.print(Lib::getSensorName(localSensorIdx));
      display.setCursor(111, y);
      display.print(Lib::ctx.values[localSensorIdx]);
      localSensorIdx = (localSensorIdx + 1) % NUM_SENSORS;
      y += 17;
    }
    drawTime();
  } while (display.nextPage());

  // Update scrolling state once per frame (after drawing)
  dispScrollOffset--;
  if (dispScrollOffset < -16) {
    dispScrollOffset = 0;
  }
  if (dispScrollOffset == 0) { sensorIDOffset = (sensorIDOffset + 1) % NUM_SENSORS; }
#endif  //DISP
}

void printUpdateScreen() {
#if defined(DISP)
  if (!displayEnabled) return;
  display.firstPage();
  do {
    display.setFont(u8g2_font_profont17_mr);
    display.setDrawColor(1);
    display.setCursor(19, 26);
    display.print(F("Updating"));
    display.setCursor(8, 42);
    display.print(F("sensors..."));

    drawTime();
  } while (display.nextPage());
#endif  //DISP
}

static void drawTime() {
#if defined(DISP)
  display.setFont(u8g2_font_profont11_mr);
  display.setDrawColor(0);
  display.drawBox(0, 0, 128, 12);
  display.setDrawColor(1);
  display.drawHLine(0, 10, 128);
  display.setCursor(0, 7);
  char buf[9];
  formatMillisTime(buf, true);
  display.print(buf);
#endif  //DISP
}

void formatMillisTime(char* buf, bool flashDots) {
  // Calculate hours, minutes, seconds based on effective millis provided by Lib
  uint32_t totalSeconds = (Lib::getTimeOfDayAsMillis()) / 1000;
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
    // Ensure the physical display is blanked when disabling output (paged)
    display.firstPage();
    do {
      // draw nothing, which results in a cleared page
    } while (display.nextPage());
  }
#endif
}

void setDisplayContrast(uint8_t value) {
#ifdef DISP
  display.setContrast(value);
#ifdef SERIAL_OUT
#ifdef DEBUG_SERIAL
  Serial.print(F("Contrast set to "));
  Serial.println((int)value);
#endif
#endif
#endif
}
}  // namespace View
