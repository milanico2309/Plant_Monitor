# Plant Monitor

## Overview
Plant Monitor is a small Arduino project that periodically reads multiple soil moisture sensors, renders output on an SH1106-based 128x64 OLED display, and sends data over the serial interface in both human-friendly and Arduino Serial Plotter formats.

- Sensor logic lives in `lib.hpp`/`lib.cpp` (namespace `Lib`).
- User I/O (serial and display) lives in `view.hpp`/`view.cpp` (namespace `View`).
- Compile-time configuration lives in `config.hpp`.
- The Arduino entry point is `Plant_Monitor.ino`.

## Features
- Configurable number of sensors via `NUM_SENSORS` (up to `MAX_SENSORS`).
- Calibratable raw-to-percent mapping using `SENSOR_CALIBRATED_MIN`/`SENSOR_CALIBRATED_MAX`.
- Optional OLED output (`DISP`) and serial outputs (`SERIAL_OUT`, `SERIAL_LOG`, `SERIAL_PLOT`).
- Lightweight, integer-only computations suitable for AVR-class MCUs.

## Building/Flashing
1. Open the project in the Arduino IDE or your preferred environment.
2. Ensure the following libraries are installed if using the display:
   - Adafruit GFX Library
   - Adafruit SH110X
3. Select your board and port, then upload.

## Doxygen Documentation
A ready-to-use `Doxyfile` is provided at the project root.

To generate HTML documentation:

```bash
# Windows (PowerShell):
doxygen .\Doxyfile
# Linux/macOS:
doxygen ./Doxyfile
```

The generated documentation will appear in `docs/html/index.html`.

## License
MIT License — see the `LICENSE` file for details.
