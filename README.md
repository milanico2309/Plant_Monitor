# Plant Monitor

## Overview

Plant Monitor is a small Arduino project that periodically reads multiple soil moisture sensors, renders output on an
SH1106-based 128x64 OLED display, and sends data over the serial interface in both human-friendly and Arduino Serial
Plotter formats.

- Sensor logic lives in `lib.hpp`/`lib.cpp` (namespace `Lib`).
- User I/O (serial and display) lives in `view.hpp`/`view.cpp` (namespace `View`).
- Compile-time configuration lives in `config.hpp`.
- The Arduino entry point is `Plant_Monitor.ino`.

## Features

- Configurable number of sensors via `NUM_SENSORS` (up to `MAX_SENSORS`).
- Calibratable raw-to-percent mapping using `SENSOR_CALIBRATED_MIN`/`SENSOR_CALIBRATED_MAX`.
- Optional OLED output (`DISP`) and serial outputs (`SERIAL_OUT`, `SERIAL_LOG`, `SERIAL_PLOT`).
- Lightweight, integer-only computations suitable for AVR-class MCUs.

## Serial Commands

The firmware accepts a small set of ASCII commands over the serial interface (line-based, LF-terminated). Commands are
case-sensitive.

- HELP
    - Description: Show a brief help message.
    - Example: HELP

- T=<ms>
    - Description: Set the effective clock to the given epoch (milliseconds). The firmware computes an internal offset
      so that the displayed/used time equals the provided value.
    - Example: T=169000000
    - Response: CMD ok: T -> <effective-ms>

- DISP=ON | DISP=OFF
    - Description: Enable or disable OLED rendering at runtime.
    - Example: DISP=OFF
    - Response: CMD ok: DISP=OFF

- CONTRAST=<v>
    - Description: Set OLED contrast (0–255).
    - Example: CONTRAST=128
    - Response: CMD ok: CONTRAST or CMD err: CONTRAST expects 0-255

- READ or READ=NOW
    - Description: Request a sensor measurement (non-blocking). The main loop will perform the measurement shortly and
      update the display/context.
    - Example: READ
    - Response: CMD ok: READ request

- PRINT or PRINT=NOW
    - Description: Print current sensor values over serial (human-readable and plotter formats depending on config).
    - Example: PRINT
    - Response: CMD ok: PRINT

Notes:

- Commands are trimmed for leading/trailing whitespace. Carriage returns (CR) are ignored; only LF ends a command.
- The serial output macros are guarded by `SERIAL_OUT`, `SERIAL_LOG` and `SERIAL_PLOT` compile-time options.

## Building/Flashing

1. Open the project in the Arduino IDE or your preferred environment.
2. Ensure the following libraries are installed if using the display:
    - Adafruit GFX Library
    - Adafruit SH110X
3. Select your board and port, then upload.

## Doxygen Documentation

A ready-to-use `Doxyfile` is provided at the project root.

To generate HTML documentation:

```cmd
:: Windows (cmd.exe)
doxygen .\Doxyfile
```

The generated documentation will appear in `docs/html/index.html`.

## License

MIT License — see the `LICENSE` file for details.
