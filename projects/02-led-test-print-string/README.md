# 02 - LCD1602 String Display

A small Keil C51 project for the STC89C52RC that initializes an LCD1602
display and prints two strings.

The current program displays `114514` on the first line and `1919810` on the
second line. Please ignore those numbers that contain internet memes.

## Hardware

- STC89C52RC or a compatible 8051 microcontroller
- LCD1602 display
- 5 V power supply
- Keil uVision with the C51 toolchain

## Source Files

- `main.c`: Initializes the LCD and writes the sample strings.
- `lcd1602.c` / `lcd1602.h`: LCD1602 command, data, and string display driver.
- `public.c` / `public.h`: Common type definitions and delay routines.

## LCD Connections

| LCD1602 Signal | 8051 Pin |
| --- | --- |
| `RS` | `P2.6` |
| `RW` | `P2.5` |
| `E` | `P2.7` |
| Data bus | `P0` |

The driver defaults to an 8-bit LCD data interface. Set
`LCD1602_4OR8_DATA_INTERFACE` to `1` in `lcd1602.h` to use a 4-bit interface.

## Build and Run

1. Open `proj.uvproj` in Keil uVision.
2. Build the project with the C51 toolchain.
3. Program `Objects/lcd1602_string_display.hex` to the STC89C52RC board.

The delay routines depend on the selected oscillator frequency. Adjust the
delay values if your board does not use a compatible clock frequency.
