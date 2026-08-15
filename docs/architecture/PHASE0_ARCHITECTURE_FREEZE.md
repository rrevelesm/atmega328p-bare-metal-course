# Phase 0 Architecture Freeze Document

This document records the baseline architectural decisions frozen during Phase 0 for the `atmega328p-bare-metal-course` repository.

## Frozen Architectural Decisions

- **Target Microcontroller:** Microchip/Atmel ATmega328P.
- **Development Paradigm:** Bare-metal C with direct register manipulation.
- **Framework Policy:** Absolute exclusion of the Arduino IDE, Arduino framework, and Arduino API functions (`digitalWrite`, `analogRead`, `delay`, `Serial`, etc.).
- **Compiler & Core Libraries:** `avr-gcc`, `avr-libc`, and `binutils-avr`.
- **Build System:** `GNU Make`.
- **Programming & Flashing Utility:** `AVRDUDE` operating over In-System Programming (ISP).
- **Primary Hardware Programmer Reference:** `USBasp`.
- **Version Control:** Git version control with a public GitHub repository model.
- **Branching Strategy:** Stable primary branch named `main`, supported by short-lived working branches (`feature/*`, `docs/*`, `fix/*`).
- **Commit Standard:** Conventional Commits specification.
- **CI Strategy:** Automated build verification in GitHub Actions without physical hardware requirements.

## Open Design Decisions

The following technical and pedagogical decisions remain intentionally open and will be evaluated and frozen in subsequent phases:

- **System Clock Frequency (`F_CPU`):** Definitive CPU frequency selection.
- **Clock Source:** Selection between external crystal oscillator (e.g., 16 MHz) versus internal RC oscillator (8 MHz).
- **Fuse Settings:** Exact values for Low (`LFUSE`), High (`HFUSE`), and Extended (`EFUSE`) fuse bytes.
- **Definitive Laboratory Hardware:** Final list of external components, sensors, and actuators for student kits.
- **Syllabus Module Structure:** Finalization of the exact 15-module pedagogical progression.
- **Integrator Project Specification:** Scope and requirements of the final bare-metal capstone project.
- **Compiler Warning Policy:** Global application of strict flags such as `-Werror`.
- **Driver Abstraction Strategy:** Rules governing custom driver modules versus direct register access in advanced practices.
- **CI Workflow Details:** Complete GitHub Actions job definitions and container environments.
- **Final Licensing Terms:** Selection and inclusion of explicit license texts (e.g., MIT for code, CC BY 4.0 for educational materials).
