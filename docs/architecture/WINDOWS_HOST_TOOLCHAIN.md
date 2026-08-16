# Windows Host Toolchain

## AVR compiler

* Distribution: Microchip AVR 8-Bit GNU Toolchain
* Version: AVR Toolchain 4.0.0
* avr-gcc version: 15.1.0
* Status: VERIFIED

## GNU Make

* Distribution: xPack Windows Build Tools v4.4.1-3
* Version: 4.4.1
* Executable path: C:\Tools\xpack-windows-build-tools-4.4.1-3\bin\make.exe
* PATH status: VERIFIED

## AVRDUDE

* Distribution: Official AVRDUDE
* Version: 8.2 (Windows x64)
* Executable path: C:\Tools\avrdude-v8.2-windows-x64\avrdude.exe
* ATmega328P definition: VERIFIED
* USBasp definition: VERIFIED
* Hardware communication: NOT TESTED

## Environment

* Host: Windows
* Shell: PowerShell
* VS Code: editor
* Arduino IDE: not required
* Microchip Studio: not required
* MPLAB X: not required

## Current verified commands

```text
avr-gcc --version
make --version
avrdude -v
```

## Pending

* build system del repositorio
* firmware real
* programación física del ATmega328P
