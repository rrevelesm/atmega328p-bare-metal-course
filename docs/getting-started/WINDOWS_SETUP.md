# Windows Toolchain & Environment Setup Guide

This document provides a step-by-step guide to installing and configuring the GNU AVR bare-metal development environment on Windows 10/11 for the **ATmega328P Bare-Metal Microcontrollers Course**.

---

## Prerequisites & Required Software Overview

The development stack consists of standalone open-source and official vendor tools. **Arduino IDE, Microchip Studio, MPLAB X, and PlatformIO are NOT required.**

| Tool | Validated Version | Official Download Source |
| :--- | :---: | :--- |
| **VS Code** | Latest | [https://code.visualstudio.com/](https://code.visualstudio.com/) |
| **C/C++ Extension** | `ms-vscode.cpptools` | [VS Code Marketplace](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) |
| **Microchip AVR GNU Toolchain** | 4.0.0 (`avr-gcc 15.1.0`) | [Microchip GCC Compilers](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers) |
| **GNU Make** | 4.4.1 (xPack) | [xPack Windows Build Tools](https://github.com/xpack-dev-tools/windows-build-tools-xpack/releases) |
| **AVRDUDE** | 8.2 (Windows x64) | [AVRDUDE GitHub Releases](https://github.com/avrdudes/avrdude/releases) |
| **Git** | 2.x+ | [https://git-scm.com/](https://git-scm.com/) |

---

## Step-by-Step Installation

### 1. Install Visual Studio Code
1. Download and run the VS Code User Installer for Windows x64.
2. Accept the defaults and complete installation.

### 2. Install VS Code C/C++ Extension
1. Open VS Code.
2. Press `Ctrl+Shift+X` to open Extensions view.
3. Search for `C/C++` (publisher: Microsoft, extension ID: `ms-vscode.cpptools`).
4. Click **Install**.

### 3. Download & Configure Microchip AVR GNU Toolchain
1. Visit the [Microchip GCC Compilers page](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers).
2. Download **AVR 8-bit GNU Toolchain 4.0.0 for Windows** (ZIP archive).
3. Extract the ZIP contents to a clean directory on your system (for example: `C:\Tools\avr8-gnu-toolchain`).
4. Locate the `bin` subfolder containing `avr-gcc.exe`, `avr-objcopy.exe`, and `avr-size.exe` (e.g. `C:\Tools\avr8-gnu-toolchain\bin`).
5. Add this `bin` directory to your Windows User **PATH** environment variable.

### 4. Download & Configure GNU Make
1. Visit the [xPack Windows Build Tools releases page](https://github.com/xpack-dev-tools/windows-build-tools-xpack/releases).
2. Download the `xpack-windows-build-tools-4.4.1-3-win32-x64.zip` archive.
3. Extract the ZIP archive to your tools folder (for example: `C:\Tools\xpack-build-tools`).
4. Locate the `bin` subfolder containing `make.exe`.
5. Add this `bin` directory to your Windows User **PATH** environment variable.

### 5. Download & Configure AVRDUDE
1. Visit the official [AVRDUDE releases page](https://github.com/avrdudes/avrdude/releases).
2. Download `avrdude-v8.2-windows-x64.zip`.
3. Extract the archive to your tools folder (for example: `C:\Tools\avrdude`).
4. Locate the folder containing `avrdude.exe` and `avrdude.conf`.
5. Add this directory to your Windows User **PATH** environment variable.

---

## Environment Verification

Open a new PowerShell window and run the following verification commands to ensure all executables are accessible from `PATH`:

```powershell
git --version
avr-gcc --version
avr-objcopy --version
avr-objdump --version
avr-size --version
make --version
avrdude --version
code --version
```

All commands must return valid version output without "command not found" errors.

---

## Workspace Setup & Compilation

1. Clone the course repository:
   ```powershell
   git clone https://github.com/rreveles/atmega328p-bare-metal-course.git
   cd atmega328p-bare-metal-course
   ```

2. Launch VS Code:
   ```powershell
   code .
   ```

3. Build firmware using GNU Make from terminal:
   ```powershell
   make
   ```

4. Alternatively, use integrated VS Code Build Tasks:
   * Press `Ctrl+Shift+B` to run **AVR: Build**.
   * Run `Terminal -> Run Task...` to execute **AVR: Size**, **AVR: Disassembly**, or **AVR: Clean**.

---

## Hardware Flashing Notice

To program physical hardware using USBasp via ISP:
```powershell
make flash
```
> **Notice:** Physical USBasp/ATmega328P programming pipeline is prepared in software; physical device validation on instructor hardware is pending.
