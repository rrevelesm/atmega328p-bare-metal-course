# Practice PR-04: External Interrupts with INT0

---

## 1. Identification

* **Practice ID:** `PR-04`
* **Practice Title:** External Interrupts with INT0
* **Module:** `M04` — Interrupt Vector Table, External Interrupts (INT0/1) & PCINT
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-03` (Configure external interrupt lines `INT0`, `INT1` and Pin Change Interrupts `PCINT` to handle asynchronous hardware events with safe atomic sections)
* **Scaffolding Level:** `Level A — Guided`
* **Difficulty:** Introductory / Intermediate

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Transition embedded firmware control logic from continuous polling loops to asynchronous hardware interrupt handling.
2. Configure external interrupt control registers (`EICRA`), mask registers (`EIMSK`), and global interrupt flags (`sei()`) on the ATmega328P.
3. Write short, non-blocking Interrupt Service Routines (`ISR(INT0_vect)`) using safe `volatile` shared event variables.
4. Explain how mechanical switch contact bounce interacts with hardware interrupts and analyze generated vector table disassembly (`build/main.lss`).

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** `PR-01` (GPIO Output), `PR-02` (GPIO Input), and `PR-03` (GPIO Debounce).
* **Required Knowledge:** Active-low input logic, interrupt vector concept, volatile qualifier usage, and SREG status register I-bit function.

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| Microcontroller | 1x | ATmega328P-PU (DIP-28 package) |
| Breadboard | 1x | Standard 830-point solderless breadboard |
| Push Button | 1x | 4-Pin Tactile Push Button |
| LED | 1x | 5mm Standard Red LED |
| Resistor | 1x | 330 Ω 1/4W Carbon Film Resistor (LED Limiter) |
| Power Supply | 1x | 5V DC regulated breadboard power supply |
| ISP Programmer | 1x | USBasp ISP programmer with cable (when available) |
| Jumper Wires | Kit | Male-to-Male breadboard jumper wires |

### Shared Laboratory Equipment
* 8-Channel USB Logic Analyzer (24 MHz) or Digital Oscilloscope (Optional for measuring interrupt response latency).

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, `attachInterrupt()`, or Wiring libraries allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| Interrupt Vector Table | Section 11: Interrupts | Reset / Vector Table | `INT0_vect` (Vector 2, Address `0x0002`) |
| External Interrupts | Section 12: External Interrupts | `EICRA`, `EIMSK`, `EIFR` | `ISC01`, `ISC00`, `INT0`, `INTF0` |
| Status Register (SREG) | Section 6.3: Status Register | `SREG` | `I`-bit (Bit 7 - Global Interrupt Enable) |

> **Datasheet Navigation Task:**  
> Open Section 12 ("External Interrupts") and Section 12.2 ("Register Description") of the ATmega328P datasheet:
> 1. Locate the physical pin number assigned to external interrupt line `INT0` on the DIP-28 package.
> 2. Determine the exact bit combination in `EICRA` required to trigger `INT0` on a **falling edge**.
> 3. Identify how the hardware clears the interrupt flag `INTF0` in register `EIFR` when the CPU jumps to `ISR(INT0_vect)`.

---

## 7. Engineering Problem Statement

Polling input pins inside a `while(1)` loop wastes CPU cycles and introduces execution latency when main loop tasks become long or complex. To capture immediate external events (e.g. sensor pulses or user interaction), embedded systems utilize hardware interrupts.

In this practice, the student must configure external interrupt `INT0` on pin `PD2` to trigger an Interrupt Service Routine (`ISR`) on a falling edge (button press to GND). The ISR must remain extremely short, setting a `volatile` shared event flag. The main loop monitors this flag and toggles status LED `PB5`.

---

## 8. Polling vs. External Interrupt Comparison

| Criterion | Polling (`PIND`) | External Interrupt (`INT0`) |
| :--- | :--- | :--- |
| **CPU Attention** | Continuous polling in loop | **Zero** (CPU responds only when event occurs) |
| **Event Latency** | Variable (Depends on loop execution time) | **Immediate** (~4 clock cycles response) |
| **Implementation Complexity**| Low | Moderate (Requires ISR & vector setup) |
| **Scalability** | Poor for multi-event systems | Excellent for event-driven architectures |
| **Switch Bounce Impact** | May miss rapid pulses | Triggers ISR on **every** falling contact bounce |

---

## 9. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `DDRD` | `0x0A` (`0x2A` I/O) | Bit 2 (`DDD2`) | Configures pin PD2 / INT0 as Input (`0`). |
| `PORTD` | `0x0B` (`0x2B` I/O) | Bit 2 (`PORTD2`) | Enables internal pull-up resistor on PD2 (`1`). |
| `DDRB` | `0x04` (`0x24` I/O) | Bit 5 (`DDB5`) | Configures pin PB5 as Output (`1`). |
| `PORTB` | `0x05` (`0x25` I/O) | Bit 5 (`PORTB5`) | Toggles status LED on confirmed press event (`^= (1 << PORTB5)`). |
| `EICRA` | `0x69` | Bits 1:0 (`ISC01`, `ISC00`) | External Interrupt Sense Control 0: `10` = Falling edge trigger. |
| `EIMSK` | `0x1D` (`0x3D` I/O) | Bit 0 (`INT0`) | External Interrupt Mask Register: Enables `INT0` interrupt line (`1`). |
| `EIFR` | `0x1C` (`0x3C` I/O) | Bit 0 (`INTF0`) | External Interrupt Flag Register: Set by hardware, cleared by ISR entry. |
| `SREG` | `0x3F` (`0x5F` I/O) | Bit 7 (`I`) | Global Interrupt Enable bit (Enabled via `sei()` macro). |

---

## 10. Circuit Schematic & Wiring Table

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `PD2 / INT0` | Pin 4 | Tactile Push Button Terminal 1 | Connected directly to PD2 |
| `GND` | Pin 8 / Pin 22 | Tactile Push Button Terminal 2 | Connected to Common Power Ground (0V) |
| `PB5` | Pin 19 | LED Anode (+) | Connected via 330 Ω series resistor |
| `GND` | Pin 8 / Pin 22 | LED Cathode (-) | Connected to Common Power Ground (0V) |
| `VCC` | Pin 7 / Pin 20 | Power Supply +5V | Connected to Regulated +5V DC Supply |
| `RESET` | Pin 1 | Pull-up Resistor | 10 kΩ connected to +5V DC |

---

## 11. Firmware Design & Algorithm

### ISR & Main Loop Execution Flow
1. Configure `PD2` as digital input with internal pull-up enabled.
2. Configure `PB5` as digital LED output.
3. Set `EICRA` bits `ISC01 = 1` and `ISC00 = 0` (Falling edge trigger on INT0).
4. Set `EIMSK` bit `INT0 = 1` (Enable INT0 interrupt mask).
5. Execute `sei()` to enable global interrupts.
6. When falling edge occurs on `PD2`:
   * CPU pauses main loop, saves Program Counter to stack.
   * Jumps to vector `INT0_vect`.
   * Executes `ISR(INT0_vect)`: sets `button_event = true;`.
   * Executes `reti` instruction to resume main loop.
7. Main loop evaluates `if (button_event)`: clears flag and toggles `PB5` LED.

---

## 12. Implementation Code

Reference source code located at `firmware/examples/04_external_interrupt/main.c`:

```c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/* Shared volatile event flag */
static volatile bool button_event = false;

/* Interrupt Service Routine for INT0 */
ISR(INT0_vect) {
    button_event = true;
}

int main(void) {
    /* 1. Configure PD2 / INT0 as input with internal pull-up */
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    /* 2. Configure PB5 as LED output */
    DDRB |= (1 << DDB5);

    /* 3. Configure INT0 for falling edge trigger */
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    /* 4. Enable INT0 interrupt line */
    EIMSK |= (1 << INT0);

    /* 5. Enable global interrupts */
    sei();

    /* 6. Main application loop */
    while (1) {
        if (button_event) {
            button_event = false;
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
make EXAMPLE_DIR=firmware/examples/04_external_interrupt clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make EXAMPLE_DIR=firmware/examples/04_external_interrupt all

# 3. Report Flash and SRAM memory consumption
make EXAMPLE_DIR=firmware/examples/04_external_interrupt size

# 4. Generate disassembly file for assembly analysis
make EXAMPLE_DIR=firmware/examples/04_external_interrupt disasm
```

---

## 14. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
make EXAMPLE_DIR=firmware/examples/04_external_interrupt flash
```

---

## 15. Verification Procedure

### A. Software Verification (Offline)
1. Verify `make` completes cleanly with return code `0` and zero compiler warnings.
2. Confirm `firmware/examples/04_external_interrupt/build/main.hex` is generated.
3. Check `make size` output: Flash text footprint is ~212 bytes, SRAM `.bss` usage is 1 byte (`button_event`).
4. Inspect `build/main.lss` vector table at offset `0x0004` to verify `jmp` instruction pointing to `__vector_1` (INT0 ISR handler).

### B. Physical Verification (With Hardware Connected)
1. Apply +5V DC power to ATmega328P.
2. Press push button connected to `PD2 / INT0`.
3. Verify LED toggles state upon button press.
4. Observe that mechanical switch bounce may cause multiple rapid ISR triggers per physical press (highlighting the need for combining interrupts with timer-based debouncing in production systems).

---

## 16. Expected Observations & Experimental Measurement

| Parameter / Signal | Theoretical Calculated Value | Measured Value | Tolerance / Error |
| :--- | :---: | :---: | :---: |
| Flash Memory Usage | 212 Bytes | 212 Bytes | 0 Bytes |
| SRAM Usage (`.bss`) | 1 Byte | 1 Byte | 0 Bytes |
| Interrupt Response Latency | 4 CPU Cycles | ~4 Cycles | N/A |

---

## 17. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. Explain the fundamental architectural difference between polling an input register versus responding to a hardware interrupt.
2. Why is physical pin `PD2` suitable for `INT0` while pin `PD4` cannot be used for `INT0`?
3. Why is falling-edge trigger (`ISC01 = 1`, `ISC00 = 0`) appropriate for a push button with internal pull-up resistor connected to GND?
4. What is the role of register `EIMSK` and what happens if `sei()` is executed without enabling `INT0` in `EIMSK`?
5. Why must an Interrupt Service Routine (`ISR`) be designed to be as short and fast as possible?
6. Explain why function calls like `_delay_ms()` or UART `printf()` should **never** be placed inside an ISR handler.
7. Why is the shared variable `button_event` declared with the `volatile` qualifier? What compiler optimization bug occurs if `volatile` is omitted?
8. Does declaring a variable `volatile` guarantee atomic read/write access on an 8-bit AVR architecture? Explain the difference between `volatile` and `atomic`.
9. Describe how mechanical switch contact bounce affects an external interrupt line configured for falling-edge detection.
10. Inspect `build/main.lss` and identify the vector table jump instruction at address `0x0004` corresponding to `INT0`.

---

## 18. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Disassembly extract of `__vectors` and `__vector_1` (INT0 ISR) from `build/main.lss`.
4. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
5. (When hardware is available) Oscilloscope or logic analyzer capture showing latency from `PD2` falling edge to `PB5` LED toggle output.

---

## 19. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles without warnings, configures INT0 correctly, and processes interrupt events. | 30 |
| **Register & ISR Control Rigor** | Correct setup of `EICRA`, `EIMSK`, `sei()`, and minimal ISR design using `volatile` flag. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, zero warnings, correct Makefile execution. | 15 |
| **Engineering & Datasheet Analysis** | Accurate answers to Datasheet Navigation Task and Analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Complete deliverables package including memory footprint and vector table disassembly. | 15 |
| **Total** | | **100** |

---

## 20. Common Failure Modes & Troubleshooting

* **Microcontroller resets when button is pressed:** An interrupt was enabled in `EIMSK` without defining the corresponding `ISR(INT0_vect)` handler, causing CPU jump to `__bad_interrupt` (vector reset).
* **ISR never executes:** Forgot to call `sei()` to enable global interrupts, or `EIMSK` bit `INT0` was not set.
* **Main loop ignores ISR events:** Shared variable `button_event` was not declared `volatile`, causing compiler to cache false value in CPU register.
* **Multiple ISR triggers per button press:** Expected behavior due to mechanical contact bounce. (Solution: combine INT0 with timer-based debouncing or hardware RC filter).

---

## 21. Workspace Cleanup & Reset State

```powershell
make EXAMPLE_DIR=firmware/examples/04_external_interrupt clean
```
Disconnect breadboard power supply after completing measurements.
