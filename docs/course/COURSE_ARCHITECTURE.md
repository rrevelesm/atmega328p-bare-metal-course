# Course Architecture: ATmega328P Bare-Metal Microcontrollers

**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  
**Course Title:** Microcontroladores ATmega328P: Programación Bare-Metal en C  
**Target Platform:** Microchip ATmega328P (DIP-28)  
**Pedagogical Philosophy:** *Datasheet -> Registros -> Periférico -> Firmware -> Medición -> Aplicación*

---

## 1. Executive Summary & Duration Scenarios

The course is designed for undergraduate engineering students (Electronic, Mechatronics, Computer Systems, Electrical Engineering). It eliminates high-level abstractions like the Arduino framework and focuses on register-level programming using GNU AVR cross-compilers (`avr-gcc`), GNU Make, and ISP programming via AVRDUDE.

### Duration Analysis

* **Scenario A (Compact - 24 Hours):** Accelerated overview covering GPIO, Timers, ADC, and UART. Suitable for short workshops; lacks depth for SPI/I2C/EEPROM and capstone projects.
* **Scenario B (Recommended - 36 Hours):** **SELECTED**. 12 hours of theory and 24 hours of hands-on laboratory practice structured across 12 modules (3 hours per module). Provides complete coverage of ATmega328P core peripherals, driver modularization, and a capstone integrator project.
* **Scenario C (Extended - 54 Hours):** Deep dive including custom bootloaders, assembly optimization, and PCB design. Exceeds standard semester time allocations for embedded systems units.

---

## 2. Selected Duration Breakdown (36 Hours Total)

* **Theory:** 12 Hours (33.3%)
* **Laboratory Practice:** 24 Hours (66.7%)
* **Structure:** 12 Modules of 3 Hours each (1h Theory / 2h Lab per module)

---

## 3. Module Breakdown Table

| ID | Module Name | Total Hours | Theory (h) | Lab (h) | Prerequisites |
| :--- | :--- | :---: | :---: | :---: | :--- |
| **M01** | AVR Harvard Architecture, Datasheet Navigation & GNU Toolchain | 3 | 1.0 | 2.0 | Basic C & Digital Logic |
| **M02** | Bare-Metal C & Bitwise Manipulation in Special Function Registers (SFR) | 3 | 1.0 | 2.0 | M01 |
| **M03** | General Purpose Input/Output (GPIO) & Software Debouncing | 3 | 1.0 | 2.0 | M02 |
| **M04** | Interrupt Vector Table, External Interrupts (INT0/1) & PCINT | 3 | 1.0 | 2.0 | M03 |
| **M05** | Timers/Counters (Timer0/Timer2) & Non-Blocking System Ticks | 3 | 1.0 | 2.0 | M04 |
| **M06** | Pulse Width Modulation (PWM) with 16-bit Timer1 | 3 | 1.0 | 2.0 | M05 |
| **M07** | Analog-to-Digital Converter (ADC) & Voltage References | 3 | 1.0 | 2.0 | M05 |
| **M08** | Universal Synchronous/Asynchronous Receiver Transmitter (USART/UART) | 3 | 1.0 | 2.0 | M07 |
| **M09** | Serial Peripheral Interface (SPI) Master/Slave Communication | 3 | 1.0 | 2.0 | M08 |
| **M10** | Two-Wire Interface (TWI / I²C) Protocol & Device Addressing | 3 | 1.0 | 2.0 | M08 |
| **M11** | Internal EEPROM Persistence, Watchdog Timer & Sleep Modes | 3 | 1.0 | 2.0 | M09, M10 |
| **M12** | Bare-Metal System Integration & Capstone Project Execution | 3 | 0.5 | 2.5 | M01-M11 |
| **Total**| **12 Modules** | **36.0** | **11.5** | **24.5** | |

---

## 4. Abstraction Progression Policy

To ensure students understand underlying hardware before abstracting it, the course enforces a strict 4-stage abstraction progression:

1. **Stage 1 (Direct Register Control - Modules 01 to 05):** All peripheral initialization and control code must be written directly inside `main()` using explicit register assignments (e.g., `DDRB |= (1 << DDB5);`).
2. **Stage 2 (Inline Helper Macros - Modules 06 to 08):** Students introduce inline functions and macro definitions to improve code readability without hiding register names.
3. **Stage 3 (Modular Drivers - Modules 09 to 11):** Students encapsulate hardware peripherals into separate `.h` and `.c` files (e.g., `uart.h`/`uart.c`, `i2c.h`/`i2c.c`). Drivers must maintain 1:1 transparency with datasheet register names.
4. **Stage 4 (Integrator Application - Module 12):** Students combine custom modular drivers in a deterministic super-loop architecture with timer tick scheduling.

---

## 5. Capstone Project Recommendation

### Project Selected: Bare-Metal Data Acquisition & Telemetry Station

* **Description:** A deterministic embedded system that reads analog sensor data via ADC, displays status via I²C/SPI, logs critical events into internal/external EEPROM, responds to external button interrupts, and transmits real-time telemetry over UART.
* **Peripherals Integrated:** GPIO, Timer0 (Tick Generator), Timer1 (PWM output), ADC, UART, I²C/SPI, EEPROM, Watchdog.
* **Justification:** Exercises all core peripherals taught in Modules 01-11 without requiring complex external OS or proprietary framework dependencies.
