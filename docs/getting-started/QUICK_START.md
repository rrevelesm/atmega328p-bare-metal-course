# Quick Start Guide

This 1-minute quick start guide assumes you have already installed `avr-gcc`, `make`, `avrdude`, and `git` on your PATH. (For complete setup instructions, see [WINDOWS_SETUP.md](WINDOWS_SETUP.md)).

---

## 1. Clone Repository & Launch VS Code

```powershell
git clone https://github.com/rreveles/atmega328p-bare-metal-course.git
cd atmega328p-bare-metal-course
code .
```

---

## 2. Compile Firmware (Terminal Method)

```powershell
# Compile default GPIO blink firmware
make

# Inspect Flash and SRAM memory footprint
make size

# Inspect AVR assembly disassembly
make disasm
```

---

## 3. Compile Firmware (VS Code Task Method)

* Press `Ctrl+Shift+B` -> Select **AVR: Build**.
* Run `Terminal -> Run Task...` -> Select **AVR: Size** or **AVR: Disassembly**.

---

## 4. Build Specific Practice Firmware

```powershell
# Compile PR-02 GPIO Input example
make EXAMPLE_DIR=firmware/examples/02_gpio_input all
```

---

## 5. Program Hardware (When USBasp is connected)

```powershell
make flash
```
