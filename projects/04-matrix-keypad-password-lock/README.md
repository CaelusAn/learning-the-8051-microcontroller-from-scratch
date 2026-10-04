# 04 - Matrix Keypad Password Lock

## Overview

This project implements a six-digit password lock on an STC89C52RC using a
4x4 matrix keypad and an LCD1602 display.

The user enters six digits, reviews the masked input, and presses `S11` to
confirm the password. A correct password unlocks the configured output pin for
five seconds. A wrong password displays a failure message and starts a
progressive lockout schedule after repeated failures.

The default password is:

```text
123456
```

Change `STORED_PASSWORD` in `password_lock.c` to set a different password.
Digits are stored separately so passwords with leading zeros are supported.

## Features

- Six-digit numeric password input.
- LCD input mask using `*` characters.
- `S11` confirmation after all six digits have been entered.
- `S12` deletes the final entered digit.
- Correct-password success state with a five-second unlock output.
- Automatic relock after the unlock period.
- Consecutive failure tracking.
- Progressive timed lockouts for repeated failures.
- Permanent disabled state after more than 30 consecutive failures.
- Timer0-based time base for long lockout periods and countdown display.
- No blocking delay is used for minute-level or hour-level lockouts.

## Keypad Mapping

The matrix keypad uses port `P1`.

```text
S1  S2  S3  S4      1  2  3  4
S5  S6  S7  S8  ->  5  6  7  8
S9  S10 S11 S12     9  0  OK DEL
S13 S14 S15 S16     invalid keys
```

Only the following keys are used by the password lock:

- `S1-S9`: digits `1-9`
- `S10`: digit `0`
- `S11`: confirm password
- `S12`: delete the final digit
- `S13-S16`: ignored

## Password Input Rules

- The password length is fixed at six digits.
- A digit is ignored after six digits have already been entered.
- Pressing `S12` removes one digit and allows entry again.
- Pressing `S11` before six digits are present does nothing.
- Pressing `S11` after six digits compares the complete password.
- Extra digits are not added while six digits are already present.

## Lockout Schedule

The password lock uses consecutive wrong-attempt counting:

- 5 failures: locked for 1 minute
- 10 failures: locked for 5 minutes
- 15 failures: locked for 10 minutes
- 20 failures: locked for 30 minutes
- 25 failures: locked for 1 hour
- 30 failures: locked for 5 hours
- More than 30 failures: permanently disabled

A successful password entry resets the consecutive failure count to zero.

The lockout values are stored in `PASSWORD_LOCK_RULES` in `password_lock.c`.

## LCD Display

The LCD1602 uses these pins:

- `RS`: `P2.6`
- `RW`: `P2.5`
- `E`: `P2.7`
- `D0-D7`: `P0.0-P0.7`

Display states:

```text
Please enter...
******
```

```text
Success
Unlocked
```

```text
Failed
Try again
```

```text
Locked
04:59
```

```text
Locked
Permanent
```

## Unlock Output

The logical lock output uses `P2.0`.

- `P2.0 = 1`: unlocked
- `P2.0 = 0`: locked

The output stays high for five seconds after a correct password. Connect a
relay or lock driver to `P2.0` if the project is extended to control a
physical lock.

## Timer0 Time Base

`timer.c` configures Timer0 as a 10 ms interval timer.

The default source configuration assumes:

- 12 MHz crystal
- Traditional 12-clock 8051 machine cycle
- Timer0 mode 1

The reload value is calculated from `TIMER0_CPU_CLOCK_HZ`. Change that constant
if the development board uses a different clock.

The timer interrupt maintains a 16-bit elapsed-seconds value. The password
lock uses timestamp differences, so minute-level and hour-level lockouts work
without long blocking delays.

## Source Files

- `main.c`: Initializes the LCD, Timer0, and password lock, then runs updates.
- `password_lock.c` / `password_lock.h`: Password lock state machine.
- `matrix_key.c` / `matrix_key.h`: 4x4 keypad scanning and key constants.
- `lcd1602.c` / `lcd1602.h`: LCD1602 command, data, and string display driver.
- `timer.c` / `timer.h`: Timer0 10 ms time base and elapsed-second counter.
- `public.c` / `public.h`: Common types and short delay routines.

## Build

1. Open `matrix_keypad_password_lock.uvproj` in Keil uVision.
2. Build the project with the C51 toolchain.
3. Program the generated HEX file:

```text
Objects/matrix_keypad_password_lock.hex
```

4. Reset the board and enter the six-digit password.

## Notes

- Only one key should be pressed at a time.
- The software delay functions are approximate and depend on the clock.
- Timer0 accuracy depends on the configured crystal frequency.
- The default stored password should be changed before real use.
- The lock output is a logic signal only; a suitable relay driver is required
  for a real electromagnetic lock.
