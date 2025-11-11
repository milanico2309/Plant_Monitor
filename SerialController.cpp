#include "WString.h"
/**
 * @file SerialController.cpp
 * @brief Implementation of the non-blocking serial command controller.
 *
 * Provides lightweight parsing and dispatching of simple ASCII commands when
 * @ref SERIAL_IN is enabled. All functions are designed to be used from the
 * Arduino main loop without blocking.
 */
#include "lib.hpp"
#include "SerialController.hpp"
#include "config.hpp"
#include "view.hpp"

#ifdef SERIAL_IN

namespace SerialController {

static char receiveBuffer[13];
static uint8_t receiveLength = 0;
static bool isLineReady = false;

// -------- helpers --------
static const char* trimAsciiWhitespace(const char* s, size_t& len) {
  // trim leading
  while (len && (*s==' ' || *s=='\t' || *s=='\r' || *s=='\n' || *s=='\v' || *s=='\f')) { ++s; --len; }
  // trim trailing
  while (len && (s[len-1]==' ' || s[len-1]=='\t' || s[len-1]=='\r' || s[len-1]=='\n' || s[len-1]=='\v' || s[len-1]=='\f')) { --len; }
  return s;
}

// -------- handlers --------
/**
 * @brief Handle the T=<ms> command to set the effective time.
 *
 * Parses a signed long integer from the argument and adjusts the global
 * millis-offset so that the effective time (Lib::getTimeOfDayAsMillis())
 * equals the provided value.
 *
 * @param arg Pointer to the ASCII argument following 'T='.
 * @return true Always returns true to indicate the command was handled
 *              (even on parse error) so the caller doesn't emit a generic
 *              "unknown command" message.
 */
static bool handleTimeCommand(const char* arg) {
  char* endp;
  long v = strtol(arg, &endp, 10);
  if (endp != arg) {
    // directly set the offset so effectiveTime == v
    long newOffset = v - (long)millis();
    Lib::setTimeOfDayMillisOffset(newOffset);

    // report effective current time over serial
    unsigned long effective = Lib::getTimeOfDayAsMillis();
    View::message(F("CMD ok: T -> "));
    View::message(effective);
    View::messageLine(F(""));
    return true;
  }
  View::messageLine(F("CMD err: bad number for T"));
  return true;
}

/**
 * @brief Handle DISP=ON|OFF command to toggle OLED rendering.
 *
 * @param arg Pointer to the argument string (expected "ON" or "OFF").
 * @return true Always returns true after reporting status.
 */
static bool handleDisplayCommand(const char* arg) {
  if (strcmp(arg, "ON") == 0) {
    View::setDisplayEnabled(true);
    View::messageLine(F("CMD ok: DISP=ON"));
    return true;
  }
  if (strcmp(arg, "OFF") == 0) {
    View::setDisplayEnabled(false);
    View::messageLine(F("CMD ok: DISP=OFF"));
    return true;
  }
  View::messageLine(F("CMD err: DISP expects ON or OFF"));
  return true;
}

/**
 * @brief Handler for READ command which requests a sensor measurement.
 *
 * The function does not perform the measurement itself; it uses
 * Lib::requestSensorRead() to notify the main loop. This keeps the
 * handler fast and non-blocking.
 */
static bool handleReadCommand(const char* /*arg*/) {
  // Use global Lib API to request a sensor read instead of local flag
  Lib::requestSensorRead();
  View::messageLine(F("CMD ok: READ request"));
  return true;
}

/**
 * @brief Handler for PRINT command which emits current values over serial.
 */
static bool handlePrintCommand(const char* /*arg*/) {
  View::valuesSerialPrint();
  View::valuesSerialPlot();
  View::messageLine(F("CMD ok: PRINT"));
  return true;
}

static bool printHelpCommands() {
  View::debugLine(F("Sending Command List!"));
  View::messageLineSerial(F("Commands:"));
  View::messageLineSerial(F("  T=<ms>        set time offset in ms"));
  View::messageLineSerial(F("  DISP=ON|OFF   enable/disable display"));
  View::messageLineSerial(F("  CONTRAST=<v>  set OLED contrast (0-255)"));
  View::messageLineSerial(F("  READ[=NOW]    trigger immediate sensor read"));
  View::messageLineSerial(F("  PRINT[=NOW]   print current values"));
  return true;
}

/**
 * @brief Handler for CONTRAST=nnn to change display contrast.
 *
 * Validates value range and reports the result over serial.
 */
static bool handleContrastCommand(const char* arg) {
  char* endp;
  long v = strtol(arg, &endp, 10);
  if (endp != arg && v >= 0 && v <= 255) {
    View::setDisplayContrast((uint8_t)v);
    View::messageLine(F("CMD ok: CONTRAST"));
    return true;
  }
  View::messageLine(F("CMD err: CONTRAST expects 0-255"));
  return true;
}

static bool dispatchCommandLine(const char* line) {
  size_t len = strlen(line);
  const char* p = trimAsciiWhitespace(line, len);

  if (len == 0) return true;

  if (strcmp(p, "HELP") == 0) {
    return printHelpCommands(); }

  if (len >= 2 && p[0]=='T' && p[1]=='=') {
    return handleTimeCommand(p + 2);
  }
  if (len >= 5 && strncmp(p, "DISP=", 5) == 0) {
    return handleDisplayCommand(p + 5);
  }
  if (len >= 9 && strncmp(p, "CONTRAST=", 9) == 0) {
    return handleContrastCommand(p + 9);
  }
  if (strcmp(p, "READ") == 0 || strcmp(p, "READ=NOW") == 0) {
    return handleReadCommand(nullptr);
  }
  if (strcmp(p, "PRINT") == 0 || strcmp(p, "PRINT=NOW") == 0) {
    return handlePrintCommand(nullptr);
  }
  return false;
}

/**
 * Public API implementations (renamed to descriptive names)
 */
void initialize() {
  // nothing to do currently; Serial is initialized in View::initSerial()
}

/**
 * @brief Poll the UART and accumulate characters until a newline is received.
 *
 * This function is non-blocking and intended to be called frequently from
 * the main loop. Carriage-returns are ignored; LF marks line completion.
 * Buffer overflow resets the input buffer to avoid ambiguous states.
 */
void pollSerial() {
#ifdef SERIAL_OUT
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue; // ignore CR
    if (c == '\n') {         // line complete
      receiveBuffer[receiveLength] = '\0';
      isLineReady = true;
      break;                  // process next time to keep this non-blocking
    }
    if ((size_t)receiveLength + 1 < sizeof(receiveBuffer)) {
      receiveBuffer[receiveLength++] = c;
    } else {
      // overflow: reset to avoid partial/ambiguous commands
      receiveLength = 0;
    }
  }
#endif // SERIAL_OUT
}

/**
 * @brief If a full line is available, parse and execute the command.
 *
 * Produces user-visible output for both success and error cases.
 */
bool processPendingCommands() {
  if (!isLineReady) return;

  bool handled = dispatchCommandLine(receiveBuffer);
  if (!handled) {
    View::messageLine(F("CMD err: unknown"));
  }
  // Reset input state
  receiveLength = 0;
  isLineReady = false;
  return handled;
}

} // namespace SerialController

#endif // SERIAL_IN
