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

`Phase 5 — Pedagogical Architecture Frozen`

## Course Structure

* **Selected Duration:** 36 Hours (12h Theory / 24h Laboratory Practice)
* **Modules:** 12 Instructional Modules
* **Practices:** 12 Hands-on Bare-Metal Laboratory Practices
* **Capstone Project:** Bare-Metal Data Acquisition & Telemetry Station
* **Architecture Specs:** See [COURSE_ARCHITECTURE.md](docs/course/COURSE_ARCHITECTURE.md)

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

## Continuous Integration

Automated hardware-independent AVR firmware compilation and verification via GitHub Actions (`.github/workflows/avr-build.yml`) running on `ubuntu-latest`.

## Institution

This material is developed for academic training activities in microcontrollers at **TecNM / Instituto Tecnológico Superior de Jerez**.

## Author

**Rafael Reveles-Martínez**
