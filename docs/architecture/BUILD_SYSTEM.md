# AVR Build System

## Build pipeline

```text
main.c
  ↓
avr-gcc (-c)
  ↓
main.o
  ↓
avr-gcc linker (-mmcu=atmega328p -Wl,-Map=main.map)
  ↓
main.elf
  ↓
avr-objcopy (-O ihex -R .eeprom)
  ↓
main.hex
```

Auxiliary analysis output generation:

```text
avr-objdump (-h -S) → main.lss (Disassembly)
avr-size (--format=berkeley) → Memory usage report
```

## Standard commands

```powershell
make         # Compiles source, generates ELF, HEX, LSS, and memory report
make clean   # Removes build artifact directory
make size    # Displays Flash/SRAM memory footprint
make disasm  # Generates assembly disassembly file (main.lss)
make flash   # Invokes AVRDUDE to program HEX via USBasp (hardware pending)
```

*Note: `make flash` requires a physical USBasp programmer connected to the target ATmega328P and has not been physically executed during Phase 2.*

## VS Code Integration

* **Primary Build Shortcut:** `Ctrl+Shift+B` executes `AVR: Build` (default task).
* **Additional Tasks:** Use `Terminal -> Run Task` to execute `AVR: Clean`, `AVR: Size`, `AVR: Disassembly`, or `AVR: Flash`.
