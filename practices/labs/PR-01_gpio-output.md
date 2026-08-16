# Practice PR-01: Basic GPIO Output & LED Control

---

## 1. Identification

* **Practice ID:** `PR-01`
* **Practice Title:** Basic GPIO Output & LED Control
* **Module:** `M03` — General Purpose Input/Output (GPIO) & Software Debouncing (Prerequisite support from `M01` and `M02`)
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-01` (Configure digital output pins using `DDRx` and `PORTx` registers and bitwise masking)
* **Scaffolding Level:** `Level A — Guided`
* **Difficulty:** Introductory

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Identify physical I/O ports (`PORTB`) and register addresses of the ATmega328P microcontroller using official Microchip datasheets.
2. Configure digital pin direction registers (`DDRB`) and output state registers (`PORTB`) using bitwise mask operators (`|=`, `^=`, `<<`) in bare-metal C.
3. Compile, inspect memory consumption (`avr-size`), generate assembly listings (`avr-objdump`), and verify execution of bare-metal C firmware without high-level framework dependencies.

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** None (Initial Pilot Practice).
* **Required Knowledge:** Basic C syntax, bitwise logic (`OR`, `XOR`, left shift), Ohm's law for LED current limiting, and binary/hexadecimal number systems.

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| Microcontroller | 1x | ATmega328P-PU (DIP-28 package) |
| Breadboard | 1x | Standard 830-point solderless breadboard |
| LED | 1x | 5mm Standard Red LED |
| Resistor | 1x | 330 Ω 1/4W Carbon Film Resistor (Current limiter) |
| Power Supply | 1x | 5V DC regulated breadboard power supply |
| ISP Programmer | 1x | USBasp ISP programmer with 10-pin/6-pin cable (when available) |
| Jumper Wires | Kit | Male-to-Male breadboard jumper wires |

### Shared Laboratory Equipment
* 8-Channel USB Logic Analyzer (24 MHz) or Digital Oscilloscope (Optional for physical pulse timing verification).

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, Wiring libraries, or `digitalWrite`/`delay` functions allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| AVR 8-bit Microcontroller Core | Section 1: Pin Configurations | N/A | Pin 19 (PB5/SCK) |
| I/O Ports Overview | Section 14: I/O-Ports | `DDRB`, `PORTB`, `PINB` | Port B Direction & Output |
| Register Description | Section 14.4: Register Description | `DDRB` (0x04), `PORTB` (0x05) | `DDB5` (Bit 5), `PORTB5` (Bit 5) |

> **Datasheet Navigation Task:**  
> Open Section 1 ("Pin Configurations") and Section 14.4 ("Register Description") of the ATmega328P datasheet. Locate:
> 1. The physical DIP-28 pin number assigned to `PB5`.
> 2. The exact hexadecimal memory addresses assigned to `DDRB` and `PORTB`.
> 3. The functional difference between bit `DDB5` in `DDRB` and bit `PORTB5` in `PORTB`.

---

## 7. Engineering Problem Statement

An embedded controller requires a visual heart-beat status indicator output on physical pin `PB5`. The indicator must toggle its logic state every 500 ms. The system must be implemented using bare-metal C by accessing Special Function Registers (SFRs) directly. High-level frameworks like Arduino are strictly forbidden due to memory overhead and lack of deterministic register control.

---

## 8. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `DDRB` | `0x04` (`0x24` I/O) | Bit 5 (`DDB5`) | Configures pin PB5 as Output (`1`) or Input (`0`). |
| `PORTB` | `0x05` (`0x25` I/O) | Bit 5 (`PORTB5`) | Sets output logic level HIGH (`1` = 5V) or LOW (`0` = 0V). |

---

## 9. Circuit Schematic & Wiring Table

> **ISP Hardware Multiplexing Warning:** Pin `PB5` (DIP-28 Pin 19) is multiplexed with the `SCK` (Serial Clock) signal used during ISP programming via USBasp. The LED load resistor **must be at least 330 Ω** to prevent excessive current loading on the `SCK` bus during firmware flashing.

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `PB5` | Pin 19 | LED Anode (+) | Connected via 330 Ω series resistor |
| `GND` | Pin 8 / Pin 22 | LED Cathode (-) | Connected to Common Power Ground (0V) |
| `VCC` | Pin 7 / Pin 20 | Power Supply +5V | Connected to Regulated +5V DC Supply |
| `RESET` | Pin 1 | Pull-up Resistor | 10 kΩ connected to +5V DC |

---

## 10. Firmware Design & Algorithm

### Control Flow Algorithm
1. Initialize execution inside `main()`.
2. Configure PB5 as a digital output pin by setting bit `DDB5` in register `DDRB` to `1` using an OR mask:  
   `DDRB |= (1 << DDB5);`
3. Enter infinite application super-loop (`while(1)`).
4. Toggle output state of bit `PORTB5` in register `PORTB` using a XOR mask:  
   `PORTB ^= (1 << PORTB5);`
5. Delay execution for 500 ms using blocking loop `_delay_ms(500);`.
6. Repeat from step 4 indefinitely.

---

## 11. Implementation Code

Reference source code located at `firmware/examples/01_gpio_blink/main.c`:

```c
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /* Step 1: Configure PB5 (DIP-28 Pin 19) as a digital output */
    DDRB |= (1 << DDB5);

    /* Step 2: Infinite application loop */
    while (1) {
        /* Toggle PB5 output logic state using XOR operator */
        PORTB ^= (1 << PORTB5);
        
        /* Delay 500 milliseconds (Requires F_CPU defined at compile time) */
        _delay_ms(500);
    }

    return 0;
}
```

---

## 12. Build Procedure

Compile the project from the repository root directory using PowerShell:

```powershell
# 1. Clean previous build artifacts
make clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make

# 3. Report Flash and SRAM memory consumption
make size

# 4. Generate disassembly file for assembly analysis
make disasm
```

---

## 13. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
# Program Flash memory of ATmega328P via USBasp
make flash
```

---

## 14. Verification Procedure

### A. Software Verification (Offline)
1. Verify that `make` completes with return code `0` and zero compiler warnings.
2. Confirm that `build/main.hex` is generated with valid Intel HEX format.
3. Check `make size` output to confirm Flash text footprint is small (~162 bytes).
4. Inspect `build/main.lss` to verify single-cycle register instruction `sbi 0x04, 5`.

### B. Physical Verification (With Hardware Connected)
1. Apply +5V DC power to ATmega328P VCC/GND pins.
2. Observe LED connected to PB5 (Pin 19) toggling ON/OFF.
3. (Optional) Connect digital oscilloscope to PB5 to measure output signal period (~1000 ms full cycle).

---

## 15. Expected Observations & Experimental Measurement

| Parameter / Signal | Theoretical Calculated Value | Measured Value | Tolerance / Error |
| :--- | :---: | :---: | :---: |
| Flash Memory Usage | 162 Bytes | 162 Bytes | 0 Bytes |
| SRAM Usage | 0 Bytes | 0 Bytes | 0 Bytes |
| Toggle Delay Period | 500 ms | ~500 ms* | $\pm 2\%$ (at 16MHz) |
| Output Voltage (HIGH) | 5.0 V DC | 4.8–5.0 V DC | $\pm 5\%$ |
| Output Voltage (LOW) | 0.0 V DC | 0.0–0.2 V DC | $+0.2\text{V}$ max |

*\*Note: If the ATmega328P is running on factory default internal RC oscillator (1 MHz) without fuses modified to 16 MHz external crystal, the measured delay will be 16 times longer (8000 ms). This clock difference is expected prior to Fuse configuration.*

---

## 16. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. What logic state does pin PB5 default to upon microcontroller reset if `DDRB` is not modified?
2. Why is the bitwise OR assignment operator (`|=`) preferred over a direct assignment (`DDRB = 0x20`) when configuring `DDB5`?
3. What numerical value does the macro expression `(1 << DDB5)` evaluate to in binary and hexadecimal?
4. Explain how the XOR operator (`^=`) achieves a state toggle on bit `PORTB5` during each iteration of the loop.
5. Why must `F_CPU` be defined before including `<util/delay.h>`? What happens if `F_CPU` does not match the physical CPU clock frequency?
6. Why does pin `PB5` require special electrical consideration when connecting external circuitry due to ISP programming?
7. Inspect `build/main.lss` and identify the AVR assembly instruction used to set bit 5 in register `DDRB` (Address `0x04`).

---

## 17. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Disassembly extract of the `main()` function from `build/main.lss`.
4. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
5. (When hardware is available) Short video or photo showing the physical breadboard setup and blinking LED.

---

## 18. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles without errors and executes expected LED toggle control. | 30 |
| **Register Control Rigor** | Correct use of `DDRB` and `PORTB` registers via bitwise masking without Arduino functions. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, clean compilation, correct Makefile execution. | 15 |
| **Engineering & Datasheet Analysis** | Accurate responses to Datasheet Navigation Task and Analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Submission of complete deliverables package including memory size and disassembly extract. | 15 |
| **Total** | | **100** |

---

## 19. Common Failure Modes & Troubleshooting

* **LED does not light up:** Check LED anode/cathode polarity. Ensure 330 Ω resistor is connected to GND.
* **LED blinks extremely slowly (8s instead of 0.5s):** Microcontroller is operating on internal 1 MHz clock while `F_CPU` is compiled for 16 MHz. (Expected behavior before Fuse configuration).
* **Compiler error `DDB5 undeclared`:** Missing `#include <avr/io.h>` header.
* **`make` command not found:** Ensure xPack GNU Make is present in system `PATH`.

---

## 20. Workspace Cleanup & Reset State

```powershell
make clean
```
Disconnect breadboard power supply after completing measurements.
