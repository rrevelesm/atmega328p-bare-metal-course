# AVR Continuous Integration (CI) Strategy

## Objective

The continuous integration pipeline automates hardware-independent build verification for all bare-metal C firmware in the repository upon every push or pull request to `main`.

## What CI Validates

* **Source Code Integrity:** Syntax correctness and strict C11 compilation via `gcc-avr`.
* **Linker Output:** Creation of binary ELF executables (`.elf`) and memory map files (`.map`).
* **Firmware Image Generation:** Successful extraction of Intel HEX binary images (`.hex`) via `avr-objcopy`.
* **Disassembly Artifacts:** Generation of assembly listing files (`.lss`) via `avr-objdump`.
* **Memory Footprint:** Reporting Flash and SRAM usage via `avr-size`.

## What CI Does NOT Validate

* Physical hardware execution on target ATmega328P microcontrollers.
* Physical USBasp programmer USB communication.
* Fuse byte configuration or physical clock oscillator frequency matching.

## Host Environments

* **Primary Teaching Environment:** Windows 10/11 using PowerShell, VS Code, Microchip AVR GNU Toolchain (Win32/x64), xPack GNU Make, and AVRDUDE.
* **Continuous Integration Environment:** GitHub Actions runner on `ubuntu-latest` using native Ubuntu Linux packages (`gcc-avr`, `avr-libc`, `binutils-avr`, `make`).

## Hardware & Flashing Policy

Continuous integration operates completely headless without physical hardware connected. The `make flash` command is **never executed** within GitHub Actions jobs.
