# Learning the 8051 Microcontroller From Scratch

A personal learning repository for the STC89C52RC and compatible 8051
microcontrollers. Each experiment is kept in its own project directory with
source code, a Keil project, and focused documentation.

> **AI Assistance and Source Acknowledgement**
>
> Codex participated in code formatting optimization and partial refactoring.
> All original code was hand-typed by me. AI optimization was intentionally
> used at publication time to improve readability. Approximately 30% of the
> code is based on or supported by existing external resources. I sincerely
> thank the providers of those code resources.

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

Project directory: `projects/02-lcd-test-print-string/`

### 03 - Matrix Keypad LCD Display

Scans a 4x4 matrix keypad through `P1` and displays the pressed key number
from `01` to `16` on an LCD1602.

Project directory: `projects/03-matrix-keypad-lcd-display/`

## Repository Layout

```text
.
|-- .editorconfig
|-- .gitattributes
|-- .gitignore
|-- CODE_STYLE.md
|-- README.md
`-- projects
    |-- 01-modular-seven-segment-demo
    |   |-- main.c
    |   |-- public.c
    |   |-- public.h
    |   |-- README.md
    |   |-- seven_segment.c
    |   |-- seven_segment.h
    |   `-- stc89c52rc_modular_demo.uvproj
    |-- 02-lcd-test-print-string
    |   |-- README.md
    |   |-- lcd1602.c
    |   |-- lcd1602.h
    |   |-- main.c
    |   |-- proj.uvproj
    |   |-- public.c
    |   `-- public.h
    `-- 03-matrix-keypad-lcd-display
        |-- README.md
        |-- lcd1602.c
        |-- lcd1602.h
        |-- main.c
        |-- matrix_key.c
        |-- matrix_key.h
        |-- matrix_keypad_lcd.uvproj
        |-- public.c
        `-- public.h
```

## Coding Style

All source code follows the conventions in [CODE_STYLE.md](CODE_STYLE.md).
The `.editorconfig` file sets the base whitespace and indentation rules for
supported editors.

## Toolchain

- Keil uVision with the C51 toolchain
- STC89C52RC development board

The included Keil target uses the AT89C52 device profile, which is compatible
with the 8051 instruction set used by the STC89C52RC.
