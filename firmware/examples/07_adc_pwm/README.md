# Firmware Example 07: ADC Acquisition & PWM Control

Bare-metal ATmega328P C firmware example demonstrating 10-bit analog signal acquisition using the internal **ADC** (`ADC0 / PC0`) and scaling the acquired voltage sample to dynamically drive **Timer1 Fast PWM** duty cycle on pin `PB1 / OC1A`.

---

## Technical Overview

* **Analog Input:** `ADC0 / PC0` (DIP-28 Pin 23) connected to potentiometer wiper.
* **Voltage Reference:** `AVCC` with external decoupling capacitor at `AREF` (`REFS0 = 1`).
* **ADC Clock:** $16\text{ MHz} / 128 = 125\text{ kHz}$ (`ADPS2:0 = 111`).
* **Digital Input Disable:** `DIDR0` bit `ADC0D` enabled to reduce power and noise on PC0.
* **Conversion Mode:** Polled single conversion (`ADSC` polling).
* **PWM Output:** `PB1 / OC1A` (DIP-28 Pin 15) running 1.0 kHz Fast PWM Mode 14 ($TOP = \text{ICR1} = 1999$).
* **Scaling:** Integer mapping $\text{OCR1A} = (\text{ADC} \times 1999) / 1023U$, mapping $0\text{--}1023 \to 0\text{--}100\%$ duty cycle.
