# Practice PR-05: Timer0 CTC Mode & 1 ms System Tick

---

## 1. Identification

* **Practice ID:** `PR-05`
* **Practice Title:** Timer0 CTC Mode & 1 ms System Tick
* **Module:** `M05` — Hardware Timers/Counters, CTC Mode & System Timebases
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-04` (Configure hardware Timers/Counters in CTC mode to generate precise periodic interrupts and non-blocking system timebases)
* **Scaffolding Level:** `Level A — Guided`
* **Difficulty:** Intermediate

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Configure an 8-bit hardware Timer/Counter (`Timer0`) in Clear Timer on Compare Match (`CTC`) mode using bare-metal control registers.
2. Derive mathematical formulas for timer prescaling, output compare match values (`OCR0A`), and periodic interrupt frequency under a given system clock (`F_CPU = 16 MHz`).
3. Construct a non-blocking 1 ms system timebase (`system_ticks_ms`) to manage concurrent execution loops without `_delay_ms()`.
4. Apply atomic blocks (`<util/atomic.h>`) to protect multi-byte shared variables accessed asynchronously across ISR and main loop contexts.

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** `PR-01` (GPIO Output), `PR-02` (GPIO Input), `PR-03` (GPIO Debounce), and `PR-04` (External Interrupts).
* **Required Knowledge:** Interrupt vector execution flow, 8-bit Timer/Counter architecture, volatile memory qualifier, and atomic multibyte access concepts.

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| Microcontroller | 1x | ATmega328P-PU (DIP-28 package) |
| Breadboard | 1x | Standard 830-point solderless breadboard |
| LED | 1x | 5mm Standard Red LED |
| Resistor | 1x | 330 Ω 1/4W Carbon Film Resistor (LED Limiter) |
| Power Supply | 1x | 5V DC regulated breadboard power supply |
| ISP Programmer | 1x | USBasp ISP programmer with cable (when available) |
| Jumper Wires | Kit | Male-to-Male breadboard jumper wires |

### Shared Laboratory Equipment
* Digital Oscilloscope or 8-Channel Logic Analyzer (For measuring LED output toggle frequency and 1 ms interrupt tick accuracy).

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, `millis()`, `TimerOne`, or Wiring functions allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| Timer0 Overview | Section 15.1: Features | `TCNT0`, `OCR0A` | 8-bit Timer/Counter |
| Modes of Operation | Section 15.7.2: Clear Timer on Compare Match (CTC) | `TCCR0A`, `TCCR0B` | `WGM02:0 = 010` (CTC) |
| Prescaler & Clock | Section 15.9: Timer/Counter Prescaler | `TCCR0B` | `CS02:0 = 011` (Prescaler 64) |
| Timer Interrupts | Section 15.9.6: TIMSK0 | `TIMSK0`, `TIFR0` | `OCIE0A`, `OCF0A` |

> **Datasheet Navigation Task:**  
> Open Section 15.7.2 ("CTC Mode") and Section 15.9 of the ATmega328P datasheet:
> 1. Locate the exact formula for compare match frequency $f_{\text{OCnx}}$ in CTC mode.
> 2. Verify why `TCNT0` resets to `0x00` automatically in the clock cycle following a compare match with `OCR0A`.
> 3. Identify the bit pattern in `TCCR0B` required to select a clock prescaler of 64 (`CS02:0`).

---

## 7. Engineering Problem Statement

Software delay loops like `_delay_ms()` freeze the CPU, preventing the microcontroller from executing other tasks. To build responsive embedded systems, microcontrollers use hardware timers to generate a periodic "system tick" (e.g., 1 ms).

In this practice, the student must configure **Timer0** in **CTC mode** to fire an interrupt (`TIMER0_COMPA_vect`) every **1.0 ms**. The ISR increments a 32-bit counter `system_ticks_ms`. The main loop reads this counter safely using atomic protection and toggles status LED `PB5` every **500 ms** without blocking the CPU.

---

## 8. Mandatory Timer Calculations & Derivations

Under the build assumption $F_{\text{CPU}} = 16,000,000\text{ Hz}$ (16 MHz):

### Step 1: Calculate Prescaler Clock Frequency
For a prescaler $N = 64$:
$$f_{\text{timer}} = \frac{F_{\text{CPU}}}{N} = \frac{16,000,000\text{ Hz}}{64} = 250,000\text{ Hz} \quad (T_{\text{timer}} = 4\,\mu\text{s})$$

### Step 2: Calculate Output Compare Match Value (`OCR0A`)
For a target interrupt frequency $f_{\text{target}} = 1,000\text{ Hz}$ ($T_{\text{target}} = 1.0\text{ ms}$):
$$f_{\text{target}} = \frac{F_{\text{CPU}}}{N \times (\text{OCR0A} + 1)}$$

$$1,000 = \frac{16,000,000}{64 \times (\text{OCR0A} + 1)} = \frac{250,000}{\text{OCR0A} + 1}$$

$$\text{OCR0A} + 1 = \frac{250,000}{1,000} = 250 \implies \mathbf{OCR0A = 249}$$

> **Verification:** 250 timer ticks (from `0` to `249`) $\times 4\,\mu\text{s per tick} = 1,000\,\mu\text{s} = 1.0\text{ ms}$.

---

## 9. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `TCCR0A` | `0x44` | Bit 1 (`WGM01`) | Configures CTC Mode (`WGM01 = 1`, `WGM00 = 0`). |
| `TCCR0B` | `0x45` | Bits 1:0 (`CS01`, `CS00`) | Selects Prescaler 64 (`CS02:0 = 011`, `WGM02 = 0`). |
| `OCR0A` | `0x47` | Bits 7:0 | Compare Value = `249` (Yields 250 counts per 1 ms interrupt). |
| `TIMSK0` | `0x6E` | Bit 1 (`OCIE0A`) | Enables Timer0 Output Compare Match A Interrupt. |
| `TCNT0` | `0x46` | Bits 7:0 | 8-bit Hardware Counter Register. |
| `DDRB` | `0x04` (`0x24` I/O) | Bit 5 (`DDB5`) | Configures pin PB5 as Output (`1`). |
| `PORTB` | `0x05` (`0x25` I/O) | Bit 5 (`PORTB5`) | Toggles status LED every 500 ms. |

---

## 10. Circuit Schematic & Wiring Table

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `PB5` | Pin 19 | LED Anode (+) | Connected via 330 Ω series resistor |
| `GND` | Pin 8 / Pin 22 | LED Cathode (-) | Connected to Common Power Ground (0V) |
| `VCC` | Pin 7 / Pin 20 | Power Supply +5V | Connected to Regulated +5V DC Supply |
| `RESET` | Pin 1 | Pull-up Resistor | 10 kΩ connected to +5V DC |

---

## 11. Firmware Design & Algorithm

### Non-Blocking Timer & System Tick Flow
1. Configure `PB5` as digital LED output.
2. Set `TCCR0A = (1 << WGM01)` for CTC mode.
3. Load `OCR0A = 249` for 1 ms periodic match.
4. Enable interrupt `TIMSK0 |= (1 << OCIE0A)`.
5. Set `TCCR0B |= (1 << CS01) | (1 << CS00)` to start Timer0 at 64 prescaler.
6. Enable global interrupts (`sei()`).
7. `ISR(TIMER0_COMPA_vect)` increments `system_ticks_ms++` every 1 ms.
8. Main loop reads `get_system_ticks_ms()` using `ATOMIC_BLOCK` and toggles LED when $(now - last) \ge 500\text{ ms}$.

---

## 12. Implementation Code

Reference source code located at `firmware/examples/05_timer0_ctc/main.c`:

```c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdbool.h>

static volatile uint32_t system_ticks_ms = 0;

ISR(TIMER0_COMPA_vect) {
    system_ticks_ms++;
}

static uint32_t get_system_ticks_ms(void) {
    uint32_t ticks;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ticks = system_ticks_ms;
    }
    return ticks;
}

int main(void) {
    DDRB |= (1 << DDB5);

    TCCR0A = (1 << WGM01);
    TCCR0B = 0;
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    TCCR0B |= (1 << CS01) | (1 << CS00);

    sei();

    uint32_t last_toggle_ms = 0;

    while (1) {
        uint32_t current_ms = get_system_ticks_ms();

        if ((current_ms - last_toggle_ms) >= 500U) {
            last_toggle_ms = current_ms;
            PORTB ^= (1 << PORTB5);
        }
    }

    return 0;
}
```

---

## 13. Build Procedure

Compile the project from the repository root directory using PowerShell:

```powershell
# 1. Clean previous build artifacts
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc all

# 3. Report Flash and SRAM memory consumption
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc size

# 4. Generate disassembly file for assembly analysis
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc disasm
```

---

## 14. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc flash
```

---

## 15. Verification Procedure & Validation Levels

### A. Software Verification (Offline)
1. Verify `make` completes with return code `0` and zero compiler warnings.
2. Confirm `firmware/examples/05_timer0_ctc/build/main.hex` is generated.
3. Check `make size` output: Flash text footprint is ~310 bytes, SRAM `.bss` usage is 4 bytes (`system_ticks_ms`).
4. Inspect `build/main.lss` vector table at offset `0x001C` (`TIMER0_COMPA_vect`) and locate `cli` / `sei` instructions generated by `ATOMIC_BLOCK`.

### B. Physical Verification (Future - With Hardware Connected)
1. Apply +5V DC power to ATmega328P.
2. Connect oscilloscope probe to `PB5` (DIP-28 Pin 19).
3. Measure LED state high/low duration (expected ~500 ms) and total square wave period (expected ~1,000 ms / 1.0 Hz).

---

## 16. Expected Observations & Physical Measurements

| Parameter / Signal | Theoretical Calculated Value | Measured Value | Tolerance / Error |
| :--- | :---: | :---: | :---: |
| Flash Memory Usage | 310 Bytes | 310 Bytes | 0 Bytes |
| SRAM Usage (`.bss`) | 4 Bytes | 4 Bytes | 0 Bytes |
| System Tick Period | 1.00 ms | 1.00 ms | Clock accuracy |
| LED Toggle Interval | 500 ms | 500 ms | $\pm 1\text{ ms}$ |
| LED Square Wave Period | 1,000 ms (1.0 Hz) | 1,000 ms (1.0 Hz) | $\pm 2\text{ ms}$ |

---

## 17. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. What is the fundamental operational difference between Timer0 Normal Mode and CTC Mode?
2. Why does setting `OCR0A = 249` produce 250 timer counts instead of 249?
3. Show the step-by-step mathematical derivation for generating a 1 ms tick using a 16 MHz clock and 64 prescaler.
4. What would be the resulting interrupt period if the same firmware ran on an ATmega328P clocked by its default internal 8 MHz RC oscillator?
5. Explain why a 32-bit variable (`uint32_t`) shared between an ISR and `main()` cannot be read atomically in a single instruction on an 8-bit AVR architecture.
6. What is the purpose of `<util/atomic.h>` and how does `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` prevent race conditions?
7. Contrast the `volatile` qualifier (which controls compiler caching) with `ATOMIC_BLOCK` (which controls hardware interrupt execution).
8. How does replacing `_delay_ms()` with a hardware system tick improve system scalability in multi-tasking bare-metal architectures?
9. Explain how the non-blocking subtraction expression `(current_ms - last_toggle_ms) >= 500U` handles 32-bit integer wraparound gracefully.
10. Inspect `build/main.lss` and identify the assembly instructions generated inside `get_system_ticks_ms()` to disable and restore global interrupts.

---

## 18. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Complete mathematical derivations for Timer0 CTC prescaler and `OCR0A` values.
4. Disassembly extract of `TIMER0_COMPA_vect` and `get_system_ticks_ms()` from `build/main.lss`.
5. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
6. (When hardware is available) Oscilloscope trace of PB5 demonstrating 1.0 Hz square wave.

---

## 19. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles without warnings, configures Timer0 CTC correctly, and generates 1 ms tick. | 30 |
| **Register & Timing Control Rigor** | Correct setup of `TCCR0A/B`, `OCR0A`, `TIMSK0`, math derivations, and atomic section usage. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, zero warnings, non-blocking architecture without `_delay_ms()`. | 15 |
| **Engineering & Datasheet Analysis** | Accurate answers to Datasheet Navigation Task, math formulas, and analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Complete deliverables package including memory footprint, math proofs, and disassembly extract. | 15 |
| **Total** | | **100** |

---

## 20. Common Failure Modes & Troubleshooting

* **LED toggles much slower than expected:** Prescaler set to 256 or 1024 instead of 64 (`CS02:0` bits misconfigured).
* **LED never toggles:** Global interrupts were not enabled (`sei()` missing) or `TIMSK0` bit `OCIE0A` was not set.
* **Corrupted tick values or unexpected jumps:** `system_ticks_ms` was read in `main()` without atomic protection, causing a race condition during 32-bit read.
* **Timer counts up to 255 instead of 249:** CTC mode was not enabled correctly (`WGM01` bit in `TCCR0A` not set).

---

## 21. Workspace Cleanup & Reset State

```powershell
make EXAMPLE_DIR=firmware/examples/05_timer0_ctc clean
```
Disconnect breadboard power supply after completing measurements.
