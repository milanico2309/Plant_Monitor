/**
 * @file Plant_Monitor.ino
 * @brief Arduino entry point for the Plant Monitor project. Handles setup and main loop.
 * @author Milan Stuhlsatz
 * @date 2025-08-21
 * @version ALPHA
 * @license MIT
 */

#include "lib.hpp"
#include "view.hpp"
#include <Wire.h>
#include <Arduino.h>
#include "SerialController.hpp"
#include <avr/interrupt.h>
#include <avr/wdt.h>


/**
 * @brief Read all sensors once and update the in-memory context.
 *
 * Also prints short debug messages to the selected outputs and shows the
 * update screen on the display (if enabled).
 */
void readSensors() {
  View::debugLine(F("Start reading"));
  View::printUpdateScreen();
  Lib::readSensorsAndUpdateMemory();
  View::debugLine(F("Reading done"));
}

/**
 * @brief Arduino setup routine.
 *
 * Initializes serial output and display (depending on configuration) and
 * prepares the sensor context and input pins.
 */
void setup() {


View::initSerial();

  uint8_t flags = MCUSR;
  Serial.print("MCUSR: 0x");
  Serial.println(flags, HEX);

  if (flags & (1 << WDRF)) Serial.println("Reset durch Watchdog (WDRF)");
  if (flags & (1 << BORF)) Serial.println("Brown-out Reset (BORF)");
  if (flags & (1 << EXTRF)) Serial.println("Externer Reset (EXTRF)");
  if (flags & (1 << PORF)) Serial.println("Power-on Reset (PORF)");

  MCUSR = 0;

View::initDisplay();
//Initialize memory
  Lib::initCtx();
  Lib::readSensorsAndUpdateMemory();
  View::debugLine(F("starting..."));

  // Configure Timer1 for READ_INTERVAL_SECONDS interrupts (ATmega328P @16MHz)
  // We'll use prescaler 1024 and compute OCR1A such that the compare match
  // interval equals READ_INTERVAL_SECONDS. READ_INTERVAL_SECONDS must be
  // small enough to fit in 16-bit OCR1A with prescaler 1024 (see config.hpp).
  noInterrupts(); // ensure atomic timer config
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12); // CTC mode (Clear Timer on Compare Match)
  // Calculate OCR1A: OCR1A = (F_CPU / prescaler) * seconds - 1
  const unsigned long ticksPerSec = (unsigned long) (F_CPU / 1024UL);
  const unsigned long ocr = (ticksPerSec * (unsigned long) READ_INTERVAL_SECONDS) - 1UL;
  OCR1A = (uint16_t) ocr;
  // set prescaler to 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable Output Compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

// Multiplier: how many base ticks are needed to reach the desired overall interval
constexpr uint8_t READ_MULTIPLIER = (READ_TARGET_SECONDS + READ_INTERVAL_SECONDS - 1) / READ_INTERVAL_SECONDS;
static_assert(READ_MULTIPLIER >= 1, "READ_MULTIPLIER calculation error");

// ISR-local counter (volatile, small) used to accumulate base intervals
static volatile uint8_t timerTickCounter = 0;

// Timer1 Compare Match A ISR: fires every READ_INTERVAL_SECONDS
ISR (TIMER1_COMPA_vect) {
  if (++timerTickCounter >= READ_MULTIPLIER) {
    timerTickCounter = 0;
    Lib::requestSensorRead();
  }
}

/**
 * @brief Arduino main loop.
 *
 * Continuously renders the main screen and triggers a
 * sensor read when requiered and sends the values over the configured serial outputs.
 */
void loop() {

#ifdef SERIAL_OUT
  SerialController::pollSerial();
  SerialController::processPendingCommands();
#endif
  if (Lib::hasSensorReadRequest()) {
    readSensors();
    View::valuesSerialPrint();
    View::valuesSerialPlot();
  }
  View::printMainScreen();
}
