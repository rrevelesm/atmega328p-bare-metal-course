# Troubleshooting Guide

Common issues encountered when setting up, compiling, or flashing ATmega328P bare-metal C firmware on Windows.

---

## 1. Toolchain & Command-Line Errors

### Issue: `'avr-gcc'` or `'make'` or `'avrdude'` is not recognized as an executable
* **Cause:** The binary installation path is missing from your Windows User `PATH` environment variable, or your terminal session was opened before PATH was modified.
* **Solution:**
  1. Verify the exact path to `bin\` for each tool (e.g., `C:\Tools\avr8-gnu-toolchain\bin`, `C:\Tools\xpack-build-tools\bin`).
  2. Open Windows Environment Variables settings and add the paths to User **PATH**.
  3. Close all PowerShell / CMD windows and VS Code instances, then relaunch a fresh terminal session.

---

## 2. VS Code & IntelliSense Errors

### Issue: Red squiggle squiggles under `#include <avr/io.h>` ("cannot open source file")
* **Cause:** VS Code IntelliSense is unable to locate `avr-libc` header directories.
* **Solution:**
  1. Open `.vscode/c_cpp_properties.json`.
  2. Verify compiler path is set to `"compilerPath": "avr-gcc.exe"`.
  3. Ensure defines include `"__AVR__"` and `"__AVR_ATmega328P__"`.
  4. Reload VS Code window (`Ctrl+Shift+P` -> `Developer: Reload Window`).

---

## 3. Clock & Timing Issues

### Issue: `_delay_ms()` produces delay times 16x longer than requested
* **Cause:** `F_CPU` is defined as `16000000UL` (16 MHz) in Makefile/code, but the target ATmega328P is running on its default factory internal RC oscillator (1 MHz).
* **Solution:** This is expected behavior before configuring physical Fuse bytes. The core GPIO logic functions correctly regardless of clock speed. Fuse programming will be addressed in advanced modules.

---

## 4. USBasp Hardware Programming Issues

### Issue: `Error: cannot find USB device with vid=0x16c0 pid=0x5dc...`
* **Status Notice:** Physical USBasp driver troubleshooting will be finalized after physical USBasp hardware is attached to the instructor machine.
* **Workaround:** Compile and verify firmware offline using `make` and `make size`.

---

## 5. Build Workspace Cleanup

### Issue: Stale build artifacts or compilation errors after changing files
```powershell
make clean
make
```
