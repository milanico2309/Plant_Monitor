/**
 * @file SerialController.hpp
 * @brief Public API for receiving and handling serial commands.
 *
 * This module is compiled in only when @ref SERIAL_IN is defined. It provides
 * non-blocking polling and processing of simple ASCII commands to control the
 * application at runtime (e.g. toggling the display, triggering reads, setting
 * time offset or display contrast).
 *
 * @ingroup serial_ctrl
 */
#pragma once
#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H

#include <Arduino.h>

/**
 * @defgroup serial_ctrl Serial Controller
 * @brief Non-blocking UART command input and dispatching.
 */
namespace SerialController {

/**
 * @brief Initialize the serial controller module.
 *
 * No hardware initialization is performed here; the global @ref Serial is
 * typically opened in @ref View::initSerial().
 *
 * @ingroup serial_ctrl
 */
void initialize();

/**
 * @brief Poll the UART and accumulate incoming command characters.
 *
 * This function is non-blocking and intended to be called frequently (each
 * iteration of the Arduino @c loop()). When a full line (terminated by @c \n)
 * has been received, it is buffered for later processing by @ref process().
 * Carriage returns (@c \r) are ignored. Buffer overflow resets the line.
 *
 * @ingroup serial_ctrl
 */
void pollSerial();

/**
 * @brief Parse and execute a complete received command line, if available.
 *
 * This function is non-blocking. If a line was completed previously by
 * @ref pollSerial(), it is dispatched here. Unknown commands produce an error
 * message.
 *
 * @return True if a command was processed, false otherwise
 * @ingroup serial_ctrl
 */
bool processPendingCommands();
} // namespace SerialController

#endif // SERIAL_CONTROLLER_H
