# 03 - Matrix Keypad LCD Display

## Overview

This project demonstrates how to scan a 4x4 matrix keypad with an STC89C52RC
microcontroller and display the pressed key number on an LCD1602 module.

After the program starts, the LCD shows `Matrix Key:` on the first row. When
the user presses one of the 16 keypad buttons, the corresponding number from
`01` to `16` appears on the second row.

The program keeps the last pressed key visible until another valid key is
detected.

## What You Will Learn

- How to scan the rows and columns of a 4x4 matrix keypad.
- How to use active-low column scanning with internal or external pull-ups.
- How to debounce a mechanical key and wait for key release.
- How to display numeric data on an LCD1602 using an 8-bit interface.
- How to split an embedded project into reusable source and header modules.

## Hardware Requirements

- STC89C52RC development board or a compatible 8051 board
- 4x4 matrix keypad connected to `P1`
- LCD1602 display
- 5 V power supply
- Keil uVision with the C51 toolchain
- USB-to-serial programmer supported by the development board

## Project Behavior

1. The program initializes the LCD1602.
2. It displays `Matrix Key:` on the first LCD row.
3. `MatrixKey()` pulls one keypad column low at a time.
4. It reads the four keypad rows to determine which button is pressed.
5. The key number is converted to a two-digit string.
6. The LCD displays the number from `01` to `16`.

For example:

```text
Matrix Key:
07
```

## Keypad Connections

The matrix keypad uses port `P1`:

| Keypad Signal | 8051 Pin | Direction |
| --- | --- | --- |
| Row 1 | `P1.7` | Input |
| Row 2 | `P1.6` | Input |
| Row 3 | `P1.5` | Input |
| Row 4 | `P1.4` | Input |
| Column 1 | `P1.3` | Output |
| Column 2 | `P1.2` | Output |
| Column 3 | `P1.1` | Output |
| Column 4 | `P1.0` | Output |

The scanning routine drives one column low while keeping the other columns
high. If a button in that column is pressed, its row line is pulled low.

## Key Number Mapping

The current firmware maps the 4x4 keypad as follows:

|  | Column 1 | Column 2 | Column 3 | Column 4 |
| --- | --- | --- | --- | --- |
| Row 1 | `01` | `02` | `03` | `04` |
| Row 2 | `05` | `06` | `07` | `08` |
| Row 3 | `09` | `10` | `11` | `12` |
| Row 4 | `13` | `14` | `15` | `16` |

## LCD Connections

The LCD1602 driver uses the following pins:

| LCD1602 Signal | 8051 Pin |
| --- | --- |
| `RS` | `P2.6` |
| `RW` | `P2.5` |
| `E` | `P2.7` |
| `D0-D7` | `P0.0-P0.7` |

The driver defaults to the 8-bit LCD data interface.

## Source Files

- `main.c`: Application entry point and main keypad display loop.
- `matrix_key.c` / `matrix_key.h`: 4x4 keypad scanning and debounce logic.
- `lcd1602.c` / `lcd1602.h`: LCD1602 command, data, and string functions.
- `public.c` / `public.h`: Common types and delay routines used by the keypad
  and LCD modules.

## Build

1. Open `matrix_keypad_lcd.uvproj` in Keil uVision.
2. Select the C51 toolchain and build the project.
3. The generated HEX file is placed in the `Objects` directory:

```text
Objects/matrix_keypad_lcd.hex
```

4. Download the HEX file to the STC89C52RC using the board programmer.

## Troubleshooting

- Confirm that the LCD first row displays `Matrix Key:`.
- Confirm that the keypad is connected to `P1`, not the independent keys on
  `P3`.
- Press only one key at a time while testing.
- Rebuild the project after changing any source file.
- Confirm that the newly generated HEX file was downloaded to the MCU.
- Check that the keypad has pull-up resistors on its row lines.
- The delay routines are calibrated only approximately. Their timing depends
  on the MCU clock frequency and compiler settings.

The independent key module on the development board uses `P3` pins, so it is
not scanned by this project.
