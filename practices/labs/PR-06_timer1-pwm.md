# Practice PR-06: Timer1 Hardware PWM Control

---

## 1. Identification

* **Practice ID:** `PR-06`
* **Practice Title:** Timer1 Hardware PWM Control
* **Module:** `M06` — Pulse Width Modulation (PWM) & Waveform Generation
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-05` (Configure 16-bit Timer1 Fast PWM mode with ICR1 TOP and OCR1A compare register to generate hardware PWM signals for servo and power control)
* **Scaffolding Level:** `Level B — Semi-Guided`
* **Difficulty:** Intermediate

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Explain the principles of Pulse Width Modulation (PWM), including period, frequency, duty cycle, and average delivered electrical power.
2. Configure 16-bit Timer/Counter1 in Fast PWM Mode 14 ($TOP = \text{ICR1}$) using bare-metal control registers (`TCCR1A`, `TCCR1B`).
3. Derive mathematical equations relating system clock ($F_{\text{CPU}} = 16\text{ MHz}$), prescaler ($N = 8$), input capture register (`ICR1`), and output compare register (`OCR1A`).
4. Implement autonomous hardware PWM generation on pin `PB1 / OC1A` without CPU intervention or Timer1 ISR execution.

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** `PR-01` (GPIO Output), `PR-02` (GPIO Input), `PR-03` (GPIO Debounce), `PR-04` (External Interrupts), and `PR-05` (Timer0 CTC System Tick).
* **Required Knowledge:** Timer prescalers, compare match hardware, 16-bit AVR register access ordering, and active-high LED power control.

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| Microcontroller | 1x | ATmega328P-PU (DIP-28 package) |
| Breadboard | 1x | Standard 830-point solderless breadboard |
| LED | 1x | 5mm Standard Red LED (PWM Indicator) |
| Resistor | 1x | 330 Ω 1/4W Carbon Film Resistor (LED Limiter) |
| Power Supply | 1x | 5V DC regulated breadboard power supply |
| ISP Programmer | 1x | USBasp ISP programmer with cable (when available) |
| Jumper Wires | Kit | Male-to-Male breadboard jumper wires |

### Shared Laboratory Equipment
* Digital Oscilloscope (100 MHz) or 8-Channel Logic Analyzer (24 MHz) for measuring PWM frequency, period, HIGH time, LOW time, and duty cycle waveforms.

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, `analogWrite()`, `Servo.h`, or Wiring libraries allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| Timer1 Block Diagram | Section 16.2: Overview | `TCNT1`, `OCR1A`, `ICR1` | 16-bit Timer/Counter1 |
| Fast PWM Mode | Section 16.9.3: Fast PWM Mode | `TCCR1A`, `TCCR1B` | `WGM13:10 = 1110` (Mode 14) |
| Compare Output Mode | Section 16.11.1: TCCR1A | `TCCR1A` | `COM1A1:0 = 10` (Non-inverting) |
| Clock Prescaler | Section 16.11.2: TCCR1B | `TCCR1B` | `CS12:0 = 010` (Prescaler 8) |

> **Datasheet Navigation Task:**  
> Open Section 16.9.3 ("Fast PWM Mode") and Section 16.11 of the ATmega328P datasheet:
> 1. Locate physical pin `PB1 / OC1A` on the DIP-28 pinout (Pin 15).
> 2. Verify why Mode 14 ($TOP = \text{ICR1}$) allows arbitrary, precise PWM frequency configuration compared to fixed 8-bit or 10-bit PWM modes.
> 3. Explain why writing to `OCR1A` in Fast PWM mode is double-buffered to prevent glitching during counter execution.

---

## 7. Engineering Problem Statement

Generating PWM signals in software by polling or toggling pins inside loops creates timing jitter and consumes 100% of CPU processing capacity. Hardware timers contain dedicated Output Compare units that switch physical output pins automatically when `TCNT1` matches `OCR1A`.

In this practice, the student must configure **Timer1** in **Fast PWM Mode 14** ($TOP = \text{ICR1}$) to generate a **1.0 kHz** PWM signal on pin `PB1 / OC1A`. The application automatically transitions through duty cycles of **25%**, **50%**, and **75%** every 2.0 seconds while the CPU remains free for other processing.

---

## 8. Pulse Width Modulation Principles

A digital PWM signal remains binary ($0\text{ V}$ or $5\text{ V}$), but varies the ratio of HIGH time ($t_{\text{HIGH}}$) to total period ($T$).

```text
25% Duty Cycle:
HIGH: ███
LOW : █████████ (Average Voltage: 1.25 V)

50% Duty Cycle:
HIGH: ██████
LOW : ██████    (Average Voltage: 2.50 V)

75% Duty Cycle:
HIGH: █████████
LOW : ███       (Average Voltage: 3.75 V)
```

> **Note on LED Brightness:** The human eye perceives average light intensity due to persistence of vision. While an LED driven by PWM toggles between 0V and 5V at 1 kHz, the perceived brightness corresponds to the average power delivered by the duty cycle. Precision quantitative validation requires an oscilloscope or logic analyzer.

---

## 9. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `DDRB` | `0x04` (`0x24` I/O) | Bit 1 (`DDB1`) | Configures pin PB1 / OC1A as Output (`1`). |
| `TCCR1A` | `0x80` | `COM1A1` (Bit 7), `WGM11` (Bit 1) | Non-inverting Fast PWM on OC1A (`COM1A1:0 = 10`, `WGM11:10 = 10`). |
| `TCCR1B` | `0x81` | `WGM13` (Bit 4), `WGM12` (Bit 3), `CS11` (Bit 1) | Mode 14 (`WGM13:12 = 11`), Prescaler 8 (`CS12:0 = 010`). |
| `ICR1` | `0x86` | Bits 15:0 | Input Capture Register: Defines TOP value (`1999`) for 1.0 kHz PWM. |
| `OCR1A` | `0x88` | Bits 15:0 | Output Compare Register A: Controls HIGH time ($0$ to `1999`). |

---

## 10. Circuit Schematic & Wiring Table

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `PB1 / OC1A` | Pin 15 | LED Anode (+) | Connected via 330 Ω series resistor |
| `GND` | Pin 8 / Pin 22 | LED Cathode (-) | Connected to Common Power Ground (0V) |
| `VCC` | Pin 7 / Pin 20 | Power Supply +5V | Connected to Regulated +5V DC Supply |
| `RESET` | Pin 1 | Pull-up Resistor | 10 kΩ connected to +5V DC |

---

## 11. Mandatory PWM Calculations & Derivations

Under $F_{\text{CPU}} = 16,000,000\text{ Hz}$ (16 MHz) and Prescaler $N = 8$:

### Step 1: Timer1 Clock Frequency
$$f_{\text{timer1}} = \frac{F_{\text{CPU}}}{N} = \frac{16,000,000\text{ Hz}}{8} = 2,000,000\text{ Hz} \quad (T_{\text{tick}} = 0.5\,\mu\text{s})$$

### Step 2: Target PWM Frequency & TOP Calculation (`ICR1`)
For $f_{\text{PWM}} = 1,000\text{ Hz}$ (1.0 kHz) in Fast PWM Mode 14:
$$f_{\text{PWM}} = \frac{F_{\text{CPU}}}{N \times (1 + \text{ICR1})}$$

$$1,000 = \frac{16,000,000}{8 \times (1 + \text{ICR1})} = \frac{2,000,000}{1 + \text{ICR1}}$$

$$1 + \text{ICR1} = \frac{2,000,000}{1,000} = 2,000 \implies \mathbf{ICR1 = 1999}$$

> **Period:** $T_{\text{PWM}} = 2,000 \times 0.5\,\mu\text{s} = 1,000\,\mu\text{s} = 1.0\text{ ms}$.

### Step 3: Duty Cycle Register Compare Values (`OCR1A`)
$$\text{OCR1A} = \frac{\text{Duty\%} \times (\text{ICR1} + 1)}{100} = \frac{\text{Duty\%} \times 2000}{100} = \text{Duty\%} \times 20$$

* **25% Duty Cycle:** $\text{OCR1A} = 25 \times 20 = \mathbf{500}$ ($t_{\text{HIGH}} = 250\,\mu\text{s}$, $t_{\text{LOW}} = 750\,\mu\text{s}$)
* **50% Duty Cycle:** $\text{OCR1A} = 50 \times 20 = \mathbf{1000}$ ($t_{\text{HIGH}} = 500\,\mu\text{s}$, $t_{\text{LOW}} = 500\,\mu\text{s}$)
* **75% Duty Cycle:** $\text{OCR1A} = 75 \times 20 = \mathbf{1500}$ ($t_{\text{HIGH}} = 750\,\mu\text{s}$, $t_{\text{LOW}} = 250\,\mu\text{s}$)

---

## 12. Firmware Design & Algorithm

### Autonomous Hardware PWM Execution
1. Configure `PB1 / OC1A` (Pin 15) as digital output.
2. Set `TCCR1A = (1 << COM1A1) | (1 << WGM11)` for non-inverting Fast PWM.
3. Set `TCCR1B = (1 << WGM13) | (1 << WGM12)` for Mode 14 ($TOP = \text{ICR1}$).
4. Set `ICR1 = 1999` ($1.0\text{ kHz}$ period).
5. Start Timer1 clock by setting `TCCR1B |= (1 << CS11)` ($N = 8$).
6. Timer1 hardware autonomously generates PWM on `PB1`. No Timer1 ISR is required.
7. Main loop updates `OCR1A` every 2.0 seconds using Timer0 system ticks.

---

## 13. Implementation Code

Reference source code located at `firmware/examples/06_timer1_pwm/main.c`:

```c
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdbool.h>

#define PWM_TOP 1999U

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

static void pwm_set_duty_percent(uint8_t duty_percent) {
    if (duty_percent > 100) {
        duty_percent = 100;
    }
    OCR1A = ((uint32_t)duty_percent * (PWM_TOP + 1U)) / 100U;
}

static void timer0_init_system_tick(void) {
    TCCR0A = (1 << WGM01);
    TCCR0B = 0;
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    TCCR0B |= (1 << CS01) | (1 << CS00);
}

static void timer1_init_pwm(void) {
    DDRB |= (1 << DDB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    ICR1 = PWM_TOP;
    pwm_set_duty_percent(50);

    TCCR1B |= (1 << CS11);
}

int main(void) {
    timer0_init_system_tick();
    timer1_init_pwm();
    sei();

    static const uint8_t duty_sequence[] = {25, 50, 75};
    uint8_t seq_index = 0;
    uint32_t last_step_ms = 0;

    while (1) {
        uint32_t current_ms = get_system_ticks_ms();

        if ((current_ms - last_step_ms) >= 2000U) {
            last_step_ms = current_ms;
            seq_index = (seq_index + 1) % 3;
            pwm_set_duty_percent(duty_sequence[seq_index]);
        }
    }

    return 0;
}
```

---

## 14. Build Procedure

Compile the project from the repository root directory using PowerShell:

```powershell
# 1. Clean previous build artifacts
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm all

# 3. Report Flash and SRAM memory consumption
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm size

# 4. Generate disassembly file for assembly analysis
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm disasm
```

---

## 15. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm flash
```

---

## 16. Verification Procedure & Measurement Plan

### A. Software Verification (Offline)
1. Verify `make` completes with return code `0` and zero compiler warnings.
2. Confirm `firmware/examples/06_timer1_pwm/build/main.hex` is generated.
3. Check `make size` output: Flash text footprint is ~528 bytes, SRAM usage is 8 bytes (`system_ticks_ms` + `duty_sequence`).
4. Inspect `build/main.lss` to verify 16-bit register writes to `ICR1` and `OCR1A` (`out` / `sts` instructions).

### B. Physical Verification (Future - With Hardware Connected)
1. Connect oscilloscope channel 1 to pin `PB1 / OC1A` (DIP-28 Pin 15).
2. Measure signal frequency (expected $1.00\text{ kHz}$) and period (expected $1.00\text{ ms}$).
3. Observe duty cycle transitions every 2.0 seconds:
   * **State 1:** $t_{\text{HIGH}} = 250\,\mu\text{s}$, $t_{\text{LOW}} = 750\,\mu\text{s}$ (25.0% Duty Cycle)
   * **State 2:** $t_{\text{HIGH}} = 500\,\mu\text{s}$, $t_{\text{LOW}} = 500\,\mu\text{s}$ (50.0% Duty Cycle)
   * **State 3:** $t_{\text{HIGH}} = 750\,\mu\text{s}$, $t_{\text{LOW}} = 250\,\mu\text{s}$ (75.0% Duty Cycle)

---

## 17. Expected Observations & Experimental Measurements

| Target Duty Cycle | Theoretical $t_{\text{HIGH}}$ | Theoretical $t_{\text{LOW}}$ | Theoretical Frequency | Measured Duty Cycle |
| :---: | :---: | :---: | :---: | :---: |
| **25.0%** | $250.0\,\mu\text{s}$ | $750.0\,\mu\text{s}$ | $1.000\text{ kHz}$ | Oscilloscope Pending |
| **50.0%** | $500.0\,\mu\text{s}$ | $500.0\,\mu\text{s}$ | $1.000\text{ kHz}$ | Oscilloscope Pending |
| **75.0%** | $750.0\,\mu\text{s}$ | $750.0\,\mu\text{s}$ | $1.000\text{ kHz}$ | Oscilloscope Pending |

---

## 18. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. Define Pulse Width Modulation (PWM) and explain how varying duty cycle alters time-average electrical power delivered to a load.
2. Why is hardware-generated PWM using Output Compare units superior to software pin-toggling in multi-tasking microcontrollers?
3. What is the role of `ICR1` in Timer1 Fast PWM Mode 14, and how does it differ from fixed 8-bit Fast PWM Mode 5?
4. Show the step-by-step mathematical derivation for generating a 1.0 kHz PWM signal using $F_{\text{CPU}} = 16\text{ MHz}$ and prescaler $N = 8$.
5. Explain why writing to `OCR1A` is double-buffered in Fast PWM mode and how double-buffering prevents pulse truncation (glitching).
6. What is the physical pin number assigned to hardware PWM output `OC1A` on the ATmega328P DIP-28 package?
7. How are 16-bit registers (`ICR1`, `OCR1A`) written safely in C on an 8-bit AVR architecture?
8. Explain why no Timer1 Interrupt Service Routine (`ISR`) is required for generating continuous PWM signals.
9. Describe what occurs if `duty_percent` is set to 0% ($\text{OCR1A} = 0$) versus 100% ($\text{OCR1A} = \text{ICR1}$).
10. Inspect `build/main.lss` and identify the assembly instructions used to write the 16-bit value into `ICR1`.

---

## 19. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Complete mathematical derivations for Timer1 Fast PWM TOP (`ICR1`) and compare match (`OCR1A`) values.
4. Disassembly extract showing 16-bit register initialization from `build/main.lss`.
5. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
6. (When hardware is available) Oscilloscope captures displaying 25%, 50%, and 75% duty cycle waveforms at 1.0 kHz.

---

## 20. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles cleanly, configures Fast PWM Mode 14, and drives OC1A hardware output. | 30 |
| **Register & PWM Control Rigor** | Correct setup of `TCCR1A/B`, `ICR1`, `OCR1A`, math derivations, and non-inverting mode. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, zero warnings, clean integer arithmetic helper function without floats. | 15 |
| **Engineering & Datasheet Analysis** | Accurate answers to Datasheet Navigation Task, PWM period math, and analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Complete deliverables package including memory footprint, math derivations, and disassembly extract. | 15 |
| **Total** | | **100** |

---

## 21. Common Failure Modes & Troubleshooting

* **No signal on output pin:** Pin `PB1` direction bit (`DDB1`) was not set to output in `DDRB`.
* **PWM frequency is incorrect:** Wrong prescaler selected in `TCCR1B` or incorrect value loaded into `ICR1`.
* **Duty cycle does not change:** `OCR1A` is being set outside the valid range $[0, \text{ICR1}]$.
* **Signal glitches during duty update:** Fast PWM double-buffering bypassed or invalid WGM mode selected.

---

## 22. Workspace Cleanup & Reset State

```powershell
make EXAMPLE_DIR=firmware/examples/06_timer1_pwm clean
```
Disconnect breadboard power supply after completing measurements.
