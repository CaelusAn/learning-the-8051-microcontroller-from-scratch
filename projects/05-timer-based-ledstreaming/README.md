# 05 - Timer-Based LED Streaming

## Overview

This project demonstrates a Timer0-based LED streaming pattern on the
STC89C52RC. The program moves one active-low LED output across port `P2` and
uses an independent key to reverse the movement direction.

The source in this directory is a commented and formatted learning snapshot.
The C statements and runtime behavior were intentionally preserved.

## Project Behavior

- Port `P2` starts at `0xFE`, so one LED output is active low.
- Timer0 generates an interrupt approximately every 1 ms.
- The interrupt counts 100 timer events before updating the LEDs.
- The LED pattern updates about every 100 ms.
- `LEDMode == 0` shifts the pattern left with `_crol_`.
- `LEDMode == 1` shifts the pattern right with `_cror_`.
- Pressing key 1 toggles the direction between left and right.

Keys 2 through 4 are returned by `Key()` but are not currently handled by
`main()`. They are available for future expansion.

## Hardware

### LEDs

The LED outputs use port `P2`.

```text
P2.0-P2.7 -> LED outputs, active low
```

### Independent Keys

The key module uses four active-low independent keys:

```text
P3.1 -> key 1
P3.0 -> key 2
P3.2 -> key 3
P3.3 -> key 4
```

The key scan includes a 20 ms press debounce and a 20 ms release debounce.

### Clock

The timer and software delay calculations assume:

```text
12 MHz crystal
12-clock 8051 machine cycle
```

## Timer Configuration

Timer0 is configured in 16-bit mode.

```text
Timer input frequency = 12 MHz / 12 = 1 MHz
One timer count          = 1 us
1 ms interrupt interval  = 1000 counts
Reload value             = 65536 - 1000 = 64536 = 0xFC18
```

The high and low bytes are written as:

```c
TL0 = 0x18;
TH0 = 0xFC;
```

The interrupt increments `T0Count`. Every 100 interrupts, the LED pattern is
shifted once.

## Source Files

- `main.c`: Main loop and Timer0 interrupt service routine.
- `Timer0.c` / `Timer0.h`: Timer0 initialization and interface.
- `Key.c` / `Key.h`: Independent key scanning and debounce logic.
- `Delay.c` / `Delay.h`: Approximate software millisecond delay.

## Build

1. Open `project.uvproj` in Keil uVision.
2. Build with the C51 toolchain.
3. Program the generated HEX file:

```text
Objects/project.hex
```

4. Reset the board and observe the LED pattern.

## Notes

- `Timer0_Routine()` is implemented in `main.c` in this learning version.
- `Timer0.c` contains the initialization routine and a short reference comment.
- Only key 1 changes the current LED behavior.
- Timer accuracy depends on the actual board clock frequency.
- The LED pattern assumes active-low LEDs on `P2`.
