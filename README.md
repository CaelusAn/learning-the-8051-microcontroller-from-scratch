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

## Repository Layout

```text
.
|-- .gitignore
|-- README.md
`-- projects
    `-- 01-modular-seven-segment-demo
        |-- Delayms.c
        |-- Delayms.h
        |-- Nixie.c
        |-- Nixie.h
        |-- main.c
        |-- README.md
        `-- stc89c52rc_modular_demo.uvproj
```

## Toolchain

- Keil uVision with the C51 toolchain
- STC89C52RC development board

The included Keil target uses the AT89C52 device profile, which is compatible
with the 8051 instruction set used by the STC89C52RC.
