# Practice PR-02: GPIO Input & Internal Pull-Up Resistor

---

## 1. Identification

* **Practice ID:** `PR-02`
* **Practice Title:** GPIO Input & Internal Pull-Up Resistor
* **Module:** `M03` — General Purpose Input/Output (GPIO) & Software Debouncing
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-02` (Read digital input states using `PINx` registers with internal pull-up resistors enabled and implement software anti-rebote algorithms)
* **Scaffolding Level:** `Level A — Guided`
* **Difficulty:** Introductory

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Configure digital input pins (`DDRD`) and enable internal pull-up resistors (`PORTD`) in the ATmega328P using direct register manipulation in bare-metal C.
2. Read the physical logic state of input pins using input pin registers (`PIND`) and process active-low logic signals from push buttons.
3. Understand the necessity of pull-up resistors in digital circuits to eliminate floating input states (`high-impedance`).
4. Inspect generated assembly code (`make disasm`) to observe hardware bit-test (`sbic`) and I/O bit clearing (`cbi`)/setting (`sbi`) instructions.

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** `PR-01` (Basic GPIO Output & LED Control).
* **Required Knowledge:** Active-low vs active-high logic, floating input states, bitwise masking AND (`&`), bitwise bit-clear (`&= ~`), and bitwise OR (`|=`).

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
* 8-Channel USB Logic Analyzer (24 MHz) or Digital Oscilloscope (Optional for contact bounce observation).

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, Wiring libraries, `digitalWrite`, `digitalRead`, or `pinMode` functions allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| AVR 8-bit Microcontroller Core | Section 1: Pin Configurations | N/A | Pin 4 (PD2 / INT0) |
| I/O Ports Configuration | Section 14.2: Ports as General Digital I/O | `DDRD`, `PORTD`, `PIND` | Pull-up Enable / Input Read |
| Register Description | Section 14.4: Register Description | `DDRD` (0x0A), `PORTD` (0x0B), `PIND` (0x09) | `DDD2` (Bit 2), `PORTD2` (Bit 2), `PIND2` (Bit 2) |

> **Datasheet Navigation Task:**  
> Open Section 14.2 ("Ports as General Digital I/O") and Section 14.4 ("Register Description") of the ATmega328P datasheet. Locate:
> 1. The physical DIP-28 pin number assigned to `PD2`.
> 2. The combination of `DDRxn` and `PORTxn` register bit values required to configure a pin as an input with an **internal pull-up resistor enabled**.
> 3. The functional difference between writing to `PORTD` versus reading from `PIND` when `DDD2 = 0`.

---

## 7. Engineering Problem Statement

An industrial control panel requires a push button input to activate a status LED. When the tactile push button connected to `PD2` is pressed, the LED on `PB5` must turn ON. When the button is released, the LED must turn OFF.

To reduce component count and PCB footprint, external pull-up resistors are omitted; the microcontroller's internal pull-up resistor on `PD2` must be enabled in firmware. High-level framework functions like `digitalRead()` or `pinMode()` are strictly prohibited.

> **Note on Mechanical Switch Bounce:** Mechanical push buttons generate electrical contact noise (bounce) during transition. In `PR-02`, we focus purely on static input state reading and internal pull-up configuration. Formal software debouncing algorithms and interrupt-driven event handling will be introduced in `PR-03`.

---

## 8. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `DDRD` | `0x0A` (`0x2A` I/O) | Bit 2 (`DDD2`) | Configures pin PD2 as Input (`0`). |
| `PORTD` | `0x0B` (`0x2B` I/O) | Bit 2 (`PORTD2`) | Enables internal pull-up resistor on PD2 (`1`) when `DDD2 = 0`. |
| `PIND` | `0x09` (`0x29` I/O) | Bit 2 (`PIND2`) | Reads physical logic state of pin PD2 (`1` = HIGH / +5V, `0` = LOW / 0V). |
| `DDRB` | `0x04` (`0x24` I/O) | Bit 5 (`DDB5`) | Configures pin PB5 as Output (`1`). |
| `PORTB` | `0x05` (`0x25` I/O) | Bit 5 (`PORTB5`) | Controls LED output state (`1` = ON, `0` = OFF). |

---

## 9. Circuit Schematic & Wiring Table

> **Active-Low Circuit Connection:** One terminal of the tactile push button is connected to `PD2` (DIP-28 Pin 4) and the opposite terminal is connected directly to `GND`. When the button is unpressed, the internal pull-up resistor holds `PD2` at +5V (HIGH). When pressed, the switch contacts close, pulling `PD2` directly to GND (LOW).

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `PD2` | Pin 4 | Tactile Push Button Terminal 1 | Connected directly to PD2 |
| `GND` | Pin 8 / Pin 22 | Tactile Push Button Terminal 2 | Connected to Common Power Ground (0V) |
| `PB5` | Pin 19 | LED Anode (+) | Connected via 330 Ω series resistor |
| `GND` | Pin 8 / Pin 22 | LED Cathode (-) | Connected to Common Power Ground (0V) |
| `VCC` | Pin 7 / Pin 20 | Power Supply +5V | Connected to Regulated +5V DC Supply |
| `RESET` | Pin 1 | Pull-up Resistor | 10 kΩ connected to +5V DC |

---

## 10. Firmware Design & Algorithm

### Control Flow Algorithm
1. Initialize execution inside `main()`.
2. Configure pin PD2 as a digital input by clearing bit `DDD2` in register `DDRD` using an AND-NOT mask:  
   `DDRD &= ~(1 << DDD2);`
3. Enable the internal pull-up resistor on PD2 by setting bit `PORTD2` in register `PORTD` to `1`:  
   `PORTD |= (1 << PORTD2);`
4. Configure pin PB5 as a digital output by setting bit `DDB5` in register `DDRB` to `1`:  
   `DDRB |= (1 << DDB5);`
5. Enter infinite application super-loop (`while(1)`).
6. Read the input state of `PIND` using bitwise AND masking: `(PIND & (1 << PIND2))`.
7. Evaluate active-low logic state:
   * If bit is `0` (button pressed to GND): Turn LED ON by setting `PORTB5` to `1`.
   * If bit is `1` (button released / pulled HIGH): Turn LED OFF by clearing `PORTB5` to `0`.
8. Repeat loop continuously.

---

## 11. Implementation Code

Reference source code located at `firmware/examples/02_gpio_input/main.c`:

```c
#include <avr/io.h>

int main(void) {
    /* Step 1: Configure PD2 (DIP-28 Pin 4) as digital input */
    DDRD &= ~(1 << DDD2);

    /* Step 2: Enable internal pull-up resistor on PD2 */
    PORTD |= (1 << PORTD2);

    /* Step 3: Configure PB5 (DIP-28 Pin 19) as digital output */
    DDRB |= (1 << DDB5);

    /* Step 4: Super-loop for polling digital input state */
    while (1) {
        /*
         * Active-Low Logic Reading:
         * Button Pressed -> PD2 connected to GND -> PIND2 is 0 -> !(0) is True -> LED ON
         * Button Released -> PD2 pulled to +5V -> PIND2 is 1 -> !(1) is False -> LED OFF
         */
        if (!(PIND & (1 << PIND2))) {
            PORTB |= (1 << PORTB5);  /* Turn LED ON */
        } else {
            PORTB &= ~(1 << PORTB5); /* Turn LED OFF */
        }
    }

    return 0;
}
```

---

## 12. Build Procedure

Compile the project from the repository root directory using PowerShell:

```powershell
# 1. Clean previous build artifacts
make EXAMPLE_DIR=firmware/examples/02_gpio_input clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make EXAMPLE_DIR=firmware/examples/02_gpio_input all

# 3. Report Flash and SRAM memory consumption
make EXAMPLE_DIR=firmware/examples/02_gpio_input size

# 4. Generate disassembly file for assembly analysis
make EXAMPLE_DIR=firmware/examples/02_gpio_input disasm
```

---

## 13. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
# Program Flash memory of ATmega328P via USBasp
make EXAMPLE_DIR=firmware/examples/02_gpio_input flash
```

---

## 14. Verification Procedure

### A. Software Verification (Offline)
1. Verify that `make` completes with return code `0` and zero compiler warnings.
2. Confirm that `firmware/examples/02_gpio_input/build/main.hex` is generated.
3. Check `make size` output to confirm Flash text footprint is small (~150 bytes).
4. Inspect `firmware/examples/02_gpio_input/build/main.lss` to locate I/O instructions `cbi 0x0a, 2` (clear DDRD bit 2), `sbi 0x0b, 2` (set PORTD bit 2), and `sbic 0x09, 2` (skip if PIND bit 2 is clear).

### B. Physical Verification (With Hardware Connected)
1. Apply +5V DC power to ATmega328P VCC/GND pins.
2. Verify LED on PB5 is OFF when push button is not pressed.
3. Press push button connected to PD2 and verify LED turns ON immediately.
4. Release push button and verify LED turns OFF.

---

## 15. Expected Observations & Experimental Measurement

| Parameter / Signal | Theoretical Calculated Value | Measured Value | Tolerance / Error |
| :--- | :---: | :---: | :---: |
| Flash Memory Usage | 150 Bytes | 150 Bytes | 0 Bytes |
| SRAM Usage | 0 Bytes | 0 Bytes | 0 Bytes |
| Input Voltage (Released - Pull-up) | 5.0 V DC | 4.8–5.0 V DC | $\pm 5\%$ |
| Input Voltage (Pressed - GND) | 0.0 V DC | 0.0–0.1 V DC | $+0.1\text{V}$ max |
| LED Response Time | Instantaneous | < 1 µs | N/A |

---

## 16. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. What happens to the logical state of a digital input pin if it is not connected to a pull-up or pull-down resistor? (Explain the term *floating input* or *high-impedance* state).
2. What is the value of the internal pull-up resistor inside the ATmega328P according to Section 28 ("Electrical Characteristics") of the datasheet?
3. Explain why pressing a button connected to GND produces a logic `0` (LOW) at the `PIND` register bit, and why this is referred to as *active-low logic*.
4. What is the exact functional difference between writing a `1` to `PORTD2` when `DDD2 = 1` (output) vs when `DDD2 = 0` (input)?
5. What numerical value does the bitwise expression `(PIND & (1 << PIND2))` evaluate to when the push button is pressed? What does it evaluate to when released?
6. Inspect `build/main.lss` and identify the AVR assembly instruction used to test bit 2 of `PIND` (`0x09`). How does the CPU handle conditional execution based on this instruction?

---

## 17. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Disassembly extract of the `main()` function from `build/main.lss`.
4. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
5. (When hardware is available) Short video or photo showing push button press controlling the LED.

---

## 18. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles cleanly and correctly controls LED state based on push button input. | 30 |
| **Register Control Rigor** | Correct configuration of `DDRD`, `PORTD`, `PIND`, and `PORTB` without Arduino API calls. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, clean compilation, correct Makefile execution. | 15 |
| **Engineering & Datasheet Analysis** | Accurate responses to Datasheet Navigation Task and Analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Submission of complete deliverables package including size report and disassembly extract. | 15 |
| **Total** | | **100** |

---

## 19. Common Failure Modes & Troubleshooting

* **LED stays ON constantly:** Push button is wired incorrectly across the same internal terminal pair, shorting PD2 to GND permanently.
* **LED behaves erratically / turns ON when hand touches wire:** Internal pull-up resistor was not enabled (`PORTD |= (1 << PORTD2)` missing), leaving PD2 in a floating high-impedance state.
* **Compiler error `PIND2 undeclared`:** Missing `#include <avr/io.h>` header.
* **Logic inverted (LED ON when released, OFF when pressed):** Omitted the logical NOT operator `!` in `if (!(PIND & (1 << PIND2)))`.

---

## 20. Workspace Cleanup & Reset State

```powershell
make EXAMPLE_DIR=firmware/examples/02_gpio_input clean
```
Disconnect breadboard power supply after completing measurements.
