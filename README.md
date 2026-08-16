# ATmega328P Bare-Metal Microcontrollers Course

[![AVR Bare-Metal Firmware CI](https://github.com/rrevelesm/atmega328p-bare-metal-course/actions/workflows/avr-build.yml/badge.svg)](https://github.com/rrevelesm/atmega328p-bare-metal-course/actions/workflows/avr-build.yml)

Academic repository for a university-level microcontroller course based on the Microchip **ATmega328P** (DIP-28), focusing on bare-metal C programming and direct register manipulation.

---

## Overview

This course delivers a datasheet-first engineering approach to 8-bit AVR microcontrollers. High-level frameworks like Arduino are deliberately excluded to ensure students master Special Function Registers (SFRs), memory maps, peripheral timing, interrupt vectors, and reproducible GNU compilation.

---

## Philosophy

* **Datasheet-First:** Register configurations derived directly from official Microchip documentation.
* **Bare-Metal Rigor:** Direct register access (`DDRx`, `PORTx`, `PINx`, SFRs) without opaque abstractions.
* **No Arduino Framework:** Zero dependency on Arduino IDE, Wiring libraries, `digitalWrite()`, or `delay()`.
* **Reproducible GNU Toolchain:** Built with `avr-gcc`, `GNU Make`, `AVRDUDE`, and VS Code.

---

## Current Capabilities

* **GPIO Output:** Digital pin control, bitwise masking (`|=`, `^=`), and assembly inspection (`PR-01`).
* **GPIO Input:** Internal pull-up activation, active-low logic, and input register reading (`PR-02`).
* **GPIO Debounce:** Non-blocking state machine filtering mechanical switch contact bounce (`PR-03`).
* **External Interrupts:** Asynchronous hardware event handling via INT0 line (`PR-04`).
* **Hardware Timers:** Timer0 CTC mode generating 1 ms atomic system timebase tick (`PR-05`).
* **Portable VS Code Workspace:** IntelliSense, task configurations, and extension recommendations.
* **Automated CI:** Headless AVR firmware build verification via GitHub Actions (`avr-build.yml`).
* **Prepared Flashing Pipeline:** AVRDUDE ISP flashing targets integrated into build scripts.

---

## Course Structure

* **Total Duration:** 36 Hours (12h Theory / 24h Laboratory Practice)
* **Instructional Modules:** 12 Modules
* **Laboratory Practices:** 12 Authored Bare-Metal Practice Guides
* **Capstone Project:** Bare-Metal Data Acquisition & Telemetry Station
* **Architecture Documentation:** See [COURSE_ARCHITECTURE.md](docs/course/COURSE_ARCHITECTURE.md)

---

## Available Practices

* [x] **PR-01 — Basic GPIO Output & LED Control:** [PR-01_gpio-output.md](practices/labs/PR-01_gpio-output.md) (Available — Level A Pilot)
* [x] **PR-02 — GPIO Input & Internal Pull-Up Resistor:** [PR-02_gpio-input.md](practices/labs/PR-02_gpio-input.md) (Available — Level A Guided)
* [x] **PR-03 — GPIO Mechanical Switch Debouncing:** [PR-03_gpio-debounce.md](practices/labs/PR-03_gpio-debounce.md) (Available — Level A Guided)
* [x] **PR-04 — External Interrupts with INT0:** [PR-04_external-interrupt.md](practices/labs/PR-04_external-interrupt.md) (Available — Level A Guided)
* [x] **PR-05 — Timer0 CTC Mode & 1 ms System Tick:** [PR-05_timer0-ctc.md](practices/labs/PR-05_timer0-ctc.md) (Available — Level A Guided)
* [ ] **PR-06 to PR-12:** Planned / In Development

---

## Getting Started

* **Quick Start (1-Minute):** See [QUICK_START.md](docs/getting-started/QUICK_START.md)
* **Windows Toolchain Setup:** See [WINDOWS_SETUP.md](docs/getting-started/WINDOWS_SETUP.md)
* **Troubleshooting Guide:** See [TROUBLESHOOTING.md](docs/getting-started/TROUBLESHOOTING.md)
* **Repository Architecture Map:** See [REPOSITORY_MAP.md](docs/REPOSITORY_MAP.md)

---

## Quick Build Command

```powershell
# Clone and build default GPIO blink example
git clone https://github.com/rrevelesm/atmega328p-bare-metal-course.git
cd atmega328p-bare-metal-course
make
make size
```

---

## Hardware Programming Status

```text
VS Code -> Make -> avr-gcc -> AVRDUDE -> USBasp (Pending physical hardware) -> ATmega328P
```

* **Status:** Software toolchain and ISP flashing scripts are fully verified; physical USBasp hardware connection on instructor machine is pending.

---

## License

This repository uses a dual-licensing model (see [LICENSING.md](docs/course/LICENSING.md)):

* **Source Code & Infrastructure:** Licensed under the [MIT License](LICENSE-CODE).
* **Educational Content & Documentation:** Licensed under [Creative Commons Attribution 4.0 International (CC BY 4.0)](LICENSE-DOCS).

---

## Author & Institutional Context

* **Author:** **Rafael Reveles-Martínez**
* **Institution:** Developed for academic training activities in microcontrollers at **TecNM / Instituto Tecnológico Superior de Jerez**.
