# STC89C52RC Modular 8051 Demo

A small Keil C51 learning project that demonstrates modular C programming for
the STC89C52RC and compatible 8051 microcontrollers.

## Modules

- `main.c`: Entry point with a periodic output toggle on `P2.0`.
- `Delayms.c` / `Delayms.h`: Millisecond delay routine calibrated for 12 MHz.
- `Nixie.c` / `Nixie.h`: Seven-segment display driver with a digit lookup table
  and position selection through `P2.2`-`P2.4`.

## Build

1. Open `stc89c52rc_modular_demo.uvproj` in Keil uVision.
2. Build the project with the C51 toolchain.
3. Program the generated HEX file to the development board.

The Keil target uses the AT89C52 device profile, which is compatible with the
8051 instruction set used by the STC89C52RC. The delay routine assumes a
12 MHz clock.
