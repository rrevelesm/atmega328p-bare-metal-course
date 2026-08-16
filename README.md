# ATmega328P Bare-Metal Microcontrollers Course

Academic repository for a microcontroller programming course based on ATmega328P using bare-metal C and direct register access.

## Philosophy

- No Arduino framework
- No Arduino API
- Datasheet-first approach
- Deep understanding of architecture and peripherals
- Reproducibility using GNU AVR tools

## Planned Toolchain

The planned development stack consists of:
- `avr-gcc`
- `avr-libc`
- `GNU Make`
- `AVRDUDE`
- `binutils-avr`

*Note: Exact toolchain versions will be audited and frozen in a subsequent phase.*

## Status

`Phase 2 — Build System & Initial Example Verified`

## Quick Build

```powershell
cd atmega328p-bare-metal-course
code .
make
```

## Hardware Programming Status

```text
VS Code -> Make -> avr-gcc -> AVRDUDE -> USBasp (Pending hardware connection) -> ATmega328P
```

Hardware programming validation: PENDING (Software toolchain fully verified; physical USBasp hardware connection pending).

## Institution

This material is developed for academic training activities in microcontrollers at **TecNM / Instituto Tecnológico Superior de Jerez**.

## Author

**Rafael Reveles-Martínez**
