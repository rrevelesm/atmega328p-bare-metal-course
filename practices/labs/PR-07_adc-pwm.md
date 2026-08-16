# Practice PR-07: ADC Acquisition & PWM Control

---

## 1. Identification

* **Practice ID:** `PR-07`
* **Practice Title:** ADC Acquisition & PWM Control
* **Module:** `M07` — Analog-to-Digital Converter (ADC) & Signal Acquisition
* **Estimated Duration:** 3.0 Hours (1.0h Theory / 2.0h Lab)
* **Associated SLO:** `SLO-06` (Configure the 10-bit ADC peripheral to acquire analog voltage signals using polling and interrupts, performing LSB scaling and digital filtering)
* **Scaffolding Level:** `Level B — Semi-Guided`
* **Difficulty:** Intermediate

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. Configure the 10-bit successive approximation Analog-to-Digital Converter (ADC) of the ATmega328P using bare-metal registers (`ADMUX`, `ADCSRA`, `DIDR0`).
2. Calculate ADC clock prescaling ($f_{\text{ADC}} = 125\text{ kHz}$), 10-bit quantization resolution ($1024$ levels), LSB size ($4.88\text{ mV}$ at $5\text{ V}$), and voltage conversion formulas.
3. Implement a single-conversion polling routine (`ADSC`) to sample analog potentiometer voltage without external libraries.
4. Scale digital ADC conversion codes ($0\text{--}1023$) to drive Timer1 Fast PWM duty cycle ($0\text{--}100\%$) using safe 32-bit integer arithmetic.

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** `PR-01` (GPIO Output), `PR-02` (GPIO Input), `PR-03` (GPIO Debounce), `PR-04` (External Interrupts), `PR-05` (Timer0 CTC), and `PR-06` (Timer1 PWM).
* **Required Knowledge:** Analog voltage division, ADC sampling and quantization concepts, 10-bit right-adjusted data alignment, and Timer1 PWM compare match control.

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| Microcontroller | 1x | ATmega328P-PU (DIP-28 package) |
| Breadboard | 1x | Standard 830-point solderless breadboard |
| Potentiometer | 1x | 10 kΩ Linear Rotary Potentiometer |
| LED | 1x | 5mm Standard Red LED (PWM Power Indicator) |
| Resistors | 2x | 330 Ω 1/4W Resistor (LED) + 100 nF Ceramic Capacitor (AREF decoupling) |
| Power Supply | 1x | 5V DC regulated breadboard power supply |
| ISP Programmer | 1x | USBasp ISP programmer with cable (when available) |
| Jumper Wires | Kit | Male-to-Male breadboard jumper wires |

### Shared Laboratory Equipment
* Digital Multimeter (DMM) and Digital Oscilloscope (100 MHz) for measuring input analog voltage ($V_{\text{IN}}$) and output PWM duty cycle.

---

## 5. Required Software & Toolchain

* **Editor:** VS Code with C/C++ Extension Pack
* **Compiler:** `avr-gcc` (Microchip AVR GNU Toolchain 15.1.0 / GCC 15+ compatible)
* **Build System:** GNU Make 4.4.1
* **Flasher:** AVRDUDE 8.2 (Windows x64)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, `analogRead()`, `analogWrite()`, or Wiring functions allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| ADC Overview | Section 24.1: Features | `ADMUX`, `ADCSRA` | 10-bit Resolution, 13-260 $\mu\text{s}$ Conv. |
| Voltage Reference | Section 24.5.1: Voltage Reference | `ADMUX` | `REFS1:0 = 01` (AVCC with cap at AREF) |
| Channel Selection | Section 24.9.1: ADMUX | `ADMUX` | `MUX3:0 = 0000` (Channel ADC0) |
| Prescaler & Enable | Section 24.9.2: ADCSRA | `ADCSRA` | `ADEN = 1`, `ADSC = 1`, `ADPS2:0 = 111` |
| Digital Input Disable | Section 24.9.4: DIDR0 | `DIDR0` | `ADC0D = 1` (Disable PC0 digital buffer) |

> **Datasheet Navigation Task:**  
> Open Section 24 ("Analog-to-Digital Converter") and Section 24.9 of the ATmega328P datasheet:
> 1. Locate physical pin `ADC0 / PC0` on the DIP-28 pinout (Pin 23).
> 2. Verify why the ADC clock $f_{\text{ADC}}$ must be configured between $50\text{ kHz}$ and $200\text{ kHz}$ to guarantee full 10-bit conversion resolution.
> 3. Explain why `ADCL` must be read before `ADCH` when accessing 10-bit results manually (and why accessing the 16-bit `ADC` macro in avr-libc automates this sequence safely).

---

## 7. Engineering Problem Statement

Real-world control systems require acquiring analog sensor signals (temperature, light, position) and translating them into actuation signals. In this practice, an analog voltage ($0\text{--}5\text{ V}$) is produced by a 10 kΩ potentiometer connected to pin `ADC0 / PC0`.

The student must configure the ATmega328P ADC peripheral to sample `ADC0` via single-conversion polling. The 10-bit digital result ($0\text{--}1023$) is scaled using integer arithmetic to adjust the compare match register `OCR1A` of Timer1 Fast PWM ($0\text{--}100\%$ duty cycle on `PB1 / OC1A`), controlling LED brightness dynamically.

---

## 8. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `ADMUX` | `0x7C` | `REFS0` (Bit 6), `MUX3:0` (Bits 3:0) | Selects AVCC reference (`REFS1:0 = 01`), Right-aligned, Channel ADC0 (`MUX = 0000`). |
| `ADCSRA` | `0x7A` | `ADEN` (Bit 7), `ADSC` (Bit 6), `ADPS2:0` (Bits 2:0) | Enables ADC (`ADEN = 1`), starts conversion (`ADSC = 1`), Prescaler 128 (`ADPS2:0 = 111`). |
| `DIDR0` | `0x7E` | Bit 0 (`ADC0D`) | Disables digital input buffer on pin PC0 / ADC0 to reduce power and noise. |
| `ADC` | `0x78` | Bits 15:0 (`ADCL` / `ADCH`) | 10-bit ADC Conversion Result Register ($0\text{ to }1023$). |
| `ICR1` | `0x86` | Bits 15:0 | Timer1 TOP value (`1999`) establishing 1.0 kHz PWM period. |
| `OCR1A` | `0x88` | Bits 15:0 | Timer1 Compare Match A: Sets PWM duty cycle proportional to ADC code. |

---

## 9. Circuit Schematic & Wiring Table

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `ADC0 / PC0` | Pin 23 | Potentiometer Wiper (Terminal 2) | Variable 0V to 5V DC Analog Input |
| `VCC` | Pin 7 / Pin 20 | Potentiometer Terminal 1 / Supply +5V | Connected to Regulated +5V DC Supply |
| `GND` | Pin 8 / Pin 22 | Potentiometer Terminal 3 / Ground | Connected to Common Power Ground (0V) |
| `AREF` | Pin 21 | 100 nF Ceramic Decoupling Capacitor | Connected between AREF and GND |
| `AVCC` | Pin 18 | Power Supply +5V | Connected directly to +5V DC (with LC filter if available) |
| `PB1 / OC1A` | Pin 15 | LED Anode (+) | Connected via 330 Ω series resistor to GND |

---

## 10. Mandatory ADC & Scaling Calculations

Under $F_{\text{CPU}} = 16,000,000\text{ Hz}$ and $V_{\text{REF}} = V_{\text{AVCC}} = 5.000\text{ V}$:

### Step 1: ADC Clock Prescaler Calculation
For maximum 10-bit resolution ($50\text{ kHz} \le f_{\text{ADC}} \le 200\text{ kHz}$), select Prescaler $N = 128$:
$$f_{\text{ADC}} = \frac{F_{\text{CPU}}}{128} = \frac{16,000,000\text{ Hz}}{128} = \mathbf{125\text{ kHz}} \quad (\text{Valid: } 50\text{ kHz} \le 125\text{ kHz} \le 200\text{ kHz})$$

### Step 2: Quantization Resolution & LSB Size
* **Total Quantization Codes ($N_{\text{codes}}$):** $2^{10} = \mathbf{1024\text{ codes}}$ (Range: $0\text{ to }1023$).
* **Ideal Step Size ($\text{LSB}$):**
$$\text{LSB} = \frac{V_{\text{REF}}}{1024} = \frac{5.000\text{ V}}{1024} = \mathbf{4.8828\text{ mV}}$$

* **Input Voltage Formula:** $V_{\text{IN}} = \frac{\text{ADC\_Code} \times V_{\text{REF}}}{1024}$

### Step 3: ADC-to-PWM Duty Scaling
Timer1 TOP value is $\text{ICR1} = 1999$ ($2000$ counts). Direct integer mapping formula:
$$\text{OCR1A} = \frac{\text{ADC\_Code} \times \text{PWM\_TOP}}{1023U} = \frac{\text{ADC\_Code} \times 1999U}{1023U}$$

| Input Voltage ($V_{\text{IN}}$) | Theoretical ADC Code | Target Duty Cycle | Scaled `OCR1A` Value |
| :---: | :---: | :---: | :---: |
| **0.00 V** | `0` | 0.0% | `0` |
| **1.25 V** | `256` | 25.0% | `500` |
| **2.50 V** | `512` | 50.0% | `1000` |
| **3.75 V** | `768` | 75.0% | `1500` |
| **5.00 V** | `1023` | 100.0% | `1999` |

---

## 11. Firmware Design & Algorithm

### Polled ADC Acquisition & PWM Update Flow
1. Configure `PB1 / OC1A` as output and set up Timer1 Fast PWM Mode 14 ($1.0\text{ kHz}$).
2. Configure `ADMUX` ($V_{\text{REF}} = \text{AVCC}$, Channel `ADC0`), `DIDR0` (`ADC0D = 1`), and `ADCSRA` ($\text{Prescaler} = 128$, $\text{ADEN} = 1$).
3. Initialize Timer0 1 ms system tick timebase and enable global interrupts (`sei()`).
4. Main loop evaluates non-blocking 50 ms sample interval:
   * Execute `adc_read()`: sets `ADSC = 1`, polls `ADSC` until cleared by hardware, reads 10-bit `ADC`.
   * Execute `pwm_set_from_adc()`: scales `ADC` code to `OCR1A` via safe 32-bit integer math.
   * Timer1 hardware automatically updates PWM output on `PB1`.

---

## 12. Implementation Code

Reference source code located at `firmware/examples/07_adc_pwm/main.c`:

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
    OCR1A = 0;
    TCCR1B |= (1 << CS11);
}

static void adc_init(void) {
    ADMUX = (1 << REFS0);
    DIDR0 |= (1 << ADC0D);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

static uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)) {
        /* Wait for conversion completion */
    }
    return ADC;
}

static void pwm_set_from_adc(uint16_t adc_val) {
    if (adc_val > 1023U) {
        adc_val = 1023U;
    }
    OCR1A = ((uint32_t)adc_val * (uint32_t)PWM_TOP) / 1023U;
}

int main(void) {
    timer0_init_system_tick();
    timer1_init_pwm();
    adc_init();
    sei();

    uint32_t last_sample_ms = 0;

    while (1) {
        uint32_t current_ms = get_system_ticks_ms();

        if ((current_ms - last_sample_ms) >= 50U) {
            last_sample_ms = current_ms;
            uint16_t raw_adc = adc_read();
            pwm_set_from_adc(raw_adc);
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
make EXAMPLE_DIR=firmware/examples/07_adc_pwm clean

# 2. Compile firmware and generate ELF, HEX, MAP, and LSS
make EXAMPLE_DIR=firmware/examples/07_adc_pwm all

# 3. Report Flash and SRAM memory consumption
make EXAMPLE_DIR=firmware/examples/07_adc_pwm size

# 4. Generate disassembly file for assembly analysis
make EXAMPLE_DIR=firmware/examples/07_adc_pwm disasm
```

---

## 14. Programming Procedure (ISP Flashing)

> **Hardware Programming Status:** Physical hardware flashing is currently **PENDING** until the USBasp programmer is attached to the host USB port. Once connected, execute:

```powershell
make EXAMPLE_DIR=firmware/examples/07_adc_pwm flash
```

---

## 15. Verification Procedure & Experimental Plan

### A. Software Verification (Offline)
1. Verify `make` completes with return code `0` and zero compiler warnings.
2. Confirm `firmware/examples/07_adc_pwm/build/main.hex` is generated.
3. Check `make size` output: Flash text footprint is ~522 bytes, SRAM usage is 4 bytes (`system_ticks_ms`).
4. Inspect `build/main.lss` to verify ADC polling loop (`sbic` / `sbi` instructions on `ADCSRA`) and integer multiplication routines.

### B. Physical Verification (Future - With Hardware Connected)
1. Connect Digital Multimeter (DMM) to pin `ADC0 / PC0` (Pin 23) and Oscilloscope probe to `PB1 / OC1A` (Pin 15).
2. Adjust potentiometer wiper across 5 test points: $0.0\text{ V}$, $1.25\text{ V}$, $2.50\text{ V}$, $3.75\text{ V}$, and $5.00\text{ V}$.
3. Verify that PWM frequency remains constant at $1.00\text{ kHz}$ while PWM duty cycle changes linearly from $0.0\%$ to $100.0\%$.

---

## 16. Expected Observations & Experimental Measurements

| Potentiometer Position | Measured $V_{\text{IN}}$ (DMM) | Expected ADC Code | Measured PWM Duty Cycle | Measured PWM Frequency |
| :---: | :---: | :---: | :---: | :---: |
| **0% (CCW)** | $0.00\text{ V}$ | `0` | $0.0\%$ | $1.00\text{ kHz}$ |
| **25%** | $1.25\text{ V}$ | `256` | $25.0\%$ | $1.00\text{ kHz}$ |
| **50% (Center)** | $2.50\text{ V}$ | `512` | $50.0\%$ | $1.00\text{ kHz}$ |
| **75%** | $3.75\text{ V}$ | `768` | $75.0\%$ | $1.00\text{ kHz}$ |
| **100% (CW)** | $5.00\text{ V}$ | `1023` | $100.0\%$ | $1.00\text{ kHz}$ |

---

## 17. Technical Engineering Analysis

Answer the following engineering questions in your lab report:
1. Explain the operational principle of a Successive Approximation Register (SAR) ADC.
2. Why does a 10-bit ADC yield 1024 quantization codes ($0\text{ to }1023$) instead of 1023 codes?
3. Calculate the exact LSB step size for an ADC operating with $V_{\text{REF}} = 5.000\text{ V}$.
4. Show why an ADC prescaler of 128 is required when $F_{\text{CPU}} = 16\text{ MHz}$ to maintain the ADC clock within the recommended $50\text{--}200\text{ kHz}$ window.
5. Explain the function of `DIDR0` bit `ADC0D` and why disabling digital input buffers on analog input pins reduces noise and power consumption.
6. What is the purpose of placing a 100 nF ceramic capacitor between pin `AREF` and `GND` when `REFS0 = 1`?
7. Contrast single-conversion polling (`ADSC` polling) with ADC interrupt-driven acquisition (`ADIE`).
8. Why is integer scaling (`OCR1A = ((uint32_t)adc * 1999) / 1023`) preferred over floating-point math (`OCR1A = (adc / 1023.0) * 1999`) in 8-bit AVR microcontrollers?
9. Explain how maximum source impedance (e.g., $10\text{ k}\Omega$ recommended max) affects the internal sample-and-hold capacitor charging time.
10. Inspect `build/main.lss` and identify the assembly polling loop instructions waiting for `ADSC` to clear in `adc_read()`.

---

## 18. Evidence Deliverables Package

Submit the following items:
1. Source code file (`main.c`).
2. Terminal output log of `make` and `make size`.
3. Complete mathematical derivations for ADC prescaler, LSB resolution, and ADC-to-PWM scaling.
4. Disassembly extract of `adc_read()` and `pwm_set_from_adc()` from `build/main.lss`.
5. Completed answers to the **Datasheet Navigation Task** and **Technical Engineering Analysis** questions.
6. (When hardware is available) Measurement table comparing DMM input voltage with oscilloscope PWM duty cycle captures.

---

## 19. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code compiles cleanly, initializes ADC0 and Timer1 Fast PWM, and scales ADC to duty cycle. | 30 |
| **Register & ADC Control Rigor** | Correct setup of `ADMUX`, `ADCSRA`, `DIDR0`, prescaler math, and polled single conversion. | 25 |
| **Code Quality & Build System** | Strict C11 formatting, zero warnings, non-blocking 50 ms sample timer, float-free integer math. | 15 |
| **Engineering & Datasheet Analysis** | Accurate answers to Datasheet Navigation Task, LSB math, and SAR ADC questions. | 15 |
| **Experimental Measurement & Evidence** | Complete deliverables package including memory footprint, math derivations, and disassembly extract. | 15 |
| **Total** | | **100** |

---

## 20. Common Failure Modes & Troubleshooting

* **ADC conversion stuck in infinite loop:** `ADEN` bit was not enabled in `ADCSRA` before setting `ADSC`.
* **ADC output reads 1023 constantly:** Potentiometer wiper is shorted to VCC or pin `PC0` digital buffer was misconfigured.
* **ADC reading fluctuates wildly:** Pin `AREF` was left floating without decoupling capacitor or `AVCC` power pin was disconnected.
* **PWM duty cycle wraps around or overflows:** 32-bit cast `(uint32_t)` was omitted during multiplication `(adc_val * PWM_TOP)`, causing 16-bit integer overflow.

---

## 21. Workspace Cleanup & Reset State

```powershell
make EXAMPLE_DIR=firmware/examples/07_adc_pwm clean
```
Disconnect breadboard power supply after completing measurements.
