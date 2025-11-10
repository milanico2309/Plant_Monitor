/**
 * @file lib.cpp
 * @brief Implementation of sensor reading and runtime context for Plant Monitor.
 */
#include "lib.hpp"
#include "config.hpp"
#include "Arduino.h"
#include "view.hpp"

namespace Lib {
  SensorContext ctx;
  static long millisOffset = 0;

  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////  FUNCTIONS  ///////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Resolve the analog pin for a given sensor index.
   * @param sensorIndex Index starting at 0.
   * @return The Arduino analog pin number or 255 if out of range.
   */
  uint8_t getSensorPin(uint8_t sensorIndex) {
    switch (sensorIndex) {
      case 0: return SENSOR_1_PIN;
      case 1: return SENSOR_2_PIN;
      case 2: return SENSOR_3_PIN;
      default: return 255;
    }
  }

  /**
   * @brief Read a sensor multiple times and return the integer-averaged value.
   * @param addr Analog pin address.
   * @return Rounded integer average of @ref AVERAGE_OF samples.
   */
  int avgRead(uint8_t addr) {
    uint16_t acc = 0; //stores values for average calculation
    for (uint8_t i = 0; i < AVERAGE_OF; i++) {
      acc += analogRead(addr); //read input value from sensor
      delay(25); //wait a moment
    }
    // integer rounded average
    return (acc + (AVERAGE_OF / 2)) / AVERAGE_OF;
  }

  /**
   * @brief Convert a raw sensor reading to a humidity percentage (0–99).
   *
   * Uses the calibrated range @ref SENSOR_CALIBRATED_MIN to
   * @ref SENSOR_CALIBRATED_MAX and clamps to [0, 99].
   * @param sensorNum Sensor index (0-based).
   * @return Percentage humidity value.
   */
  int getHumidity(const int sensorNum) {
    int raw = avgRead(getSensorPin(sensorNum));
    int span = (int) SENSOR_CALIBRATED_MAX - (int) SENSOR_CALIBRATED_MIN;
    int pct = 100 - ((raw - (int) SENSOR_CALIBRATED_MIN) * 100) / span; // integer math
    int val = constrain(pct, 0, 99); //limit value to between 0 and 99%
    return val;
  }

  /**
   * @brief Read all sensors and write results to the global context @ref ctx.
   */
  void readSensorsAndUpdateMemory() {
    for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
      ctx.values[sensorNum] = getHumidity(sensorNum);
    }
  }

  /**
   * @brief Return sensor name stored in flash for an index.
   * @param idx 0-based sensor index.
   * @return Pointer to flash-stored name; "?" if out of range.
   */
  const __FlashStringHelper *getSensorName(uint8_t idx) {
    switch (idx) {
#if NUM_SENSORS >= 1
      case 0: return SENSOR_1_ID;
#endif
#if NUM_SENSORS >= 2
      case 1: return SENSOR_2_ID;
#endif
#if NUM_SENSORS >= 3
      case 2: return SENSOR_3_ID;
#endif
      default: return F("?");
    }
  }

  /**
 * @brief Setze den Millisekunden-Offset für die Effektivzeit.
 *
 * Die effektive Zeit, die an anderen Modulen (z.B. View) zurückgegeben
 * wird, ist millis() + offset. Der Offset ist signiert und kann auch
 * negative Werte annehmen.
 *
 * @param offset Signierter Offset in Millisekunden, der zu millis() addiert wird.
 * @note Diese Funktion ist nicht als ISR-sicher gedacht; sie wird typischerweise
 *       aus dem Hauptprogramm oder einer seriellen Eingabe heraus aufgerufen.
 */
  void setMillisOffset(long offset) {
    millisOffset = offset;
  }

  /**
   * @brief Liefert die aktuelle, effektive Zeit in Millisekunden.
   *
   * Berechnet millis() + Offset und gibt das Ergebnis mit denselben
   * Semantiken wie Arduino::millis() (unsigned long) zurück.
   *
   * @return unsigned long Effektive Zeit in Millisekunden.
   */
  unsigned long getEffectiveMillis() {
    // ensure same unsigned long semantics as Arduino::millis()
    return (unsigned long) ((long) millis() + millisOffset);
  }

  // Global sensor-read request flag. volatile so it can be set from ISRs or other modules.
  static volatile uint8_t sensorReadRequested = 0;

  /**
   * @brief Fordere eine Sensormessung an (als nicht-blockierendes Flag).
   *
   * Diese Funktion setzt ein Atom-Flag, das in der Hauptschleife abgefragt
   * und konsumiert werden kann. Sie ist speziell so implementiert, dass
   * bei 8-Bit-AVRs ein Byte-Schreibzugriff atomar ist und direkt aus einer
   * ISR aufgerufen werden kann.
   *
   * Aufruf-Contract:
   * - Kann aus ISR oder normalem Kontext aufgerufen werden.
   * - Setzt internal ein Flag; führt die Messung nicht direkt durch.
   */
  void requestSensorRead() {
    sensorReadRequested = 1;
  }

  /**
   * @brief Atomar abfragen und zurücksetzen, ob eine Sensormessung angefordert wurde.
   *
   * Diese Funktion reserviert die Anfrage (consume) in einem atomaren Block
   * und gibt zurück, ob vor dem Aufruf eine Messanforderung gestellt wurde.
   *
   * @return true  Wenn eine Messanforderung vorhanden war (und nun gelöscht wurde).
   * @return false Wenn keine Messanforderung vorhanden war.
   */
  bool getAndClearSensorReadRequest() {
    bool wasRequested;
    noInterrupts();
    wasRequested = (sensorReadRequested != 0);
    sensorReadRequested = 0;
    interrupts();
    return wasRequested;
  }

  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////    SETUP    ///////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Initialize the context and configure sensor input pins.
   */
  void initCtx() {
    ctx = {
      .values = {0, 0, 0}
    };

    for (uint8_t sensorNum = 0; sensorNum < NUM_SENSORS; sensorNum++) {
      pinMode(getSensorPin(sensorNum), INPUT);
    }
  }
} // namespace Lib
