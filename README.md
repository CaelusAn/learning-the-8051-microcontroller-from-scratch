# Learning the 8051 Microcontroller From Scratch

A personal learning repository for the STC89C52RC and compatible 8051
microcontrollers. Each experiment is kept in its own project directory with
source code, a Keil project, and focused documentation.

## Projects

### 01 - Modular Seven-Segment Demo

Demonstrates modular C programming with separate delay and seven-segment
display drivers. The program includes an output toggle on `P2.0`, while the
display module provides a digit lookup table and position selection through
`P2.2`-`P2.4`.

Project directory: `projects/01-modular-seven-segment-demo/`

### 02 - LCD1602 String Display

Initializes an LCD1602 display and prints two demo strings. The project also
contains reusable delay helpers and an LCD command/data driver.

Project directory: `projects/02-led-test-print-string/`

### 03 - Matrix Keypad LCD Display

Scans a 4x4 matrix keypad through `P1` and displays the pressed key number
from `01` to `16` on an LCD1602.

Project directory: `projects/03-matrix-keypad-lcd-display/`

## Repository Layout

```text
.
|-- .gitignore
|-- README.md
`-- projects
    |-- 01-modular-seven-segment-demo
    |   |-- Delayms.c
    |   |-- Delayms.h
    |   |-- Nixie.c
    |   |-- Nixie.h
    |   |-- main.c
    |   |-- README.md
    |   `-- stc89c52rc_modular_demo.uvproj
    |-- 02-led-test-print-string
    |   |-- README.md
    |   |-- lcd1602.c
    |   |-- lcd1602.h
    |   |-- main.c
    |   |-- proj.uvproj
    |   |-- public.c
    |   `-- public.h
    `-- 03-matrix-keypad-lcd-display
        |-- README.md
        |-- Delayms.c
        |-- Delayms.h
        |-- lcd1602.c
        |-- lcd1602.h
        |-- main.c
        |-- matrix_key.c
        |-- matrix_key.h
        |-- matrix_keypad_lcd.uvproj
        |-- public.c
        `-- public.h
```

## Toolchain

- Keil uVision with the C51 toolchain
- STC89C52RC development board

The included Keil target uses the AT89C52 device profile, which is compatible
with the 8051 instruction set used by the STC89C52RC.
