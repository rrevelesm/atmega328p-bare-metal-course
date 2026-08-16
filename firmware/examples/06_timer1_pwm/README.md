# Firmware Example 06: Timer1 Hardware PWM

Bare-metal ATmega328P C firmware example demonstrating autonomous hardware Pulse Width Modulation (PWM) generation using **Timer/Counter1** in **Fast PWM Mode 14** ($TOP = \text{ICR1}$).

---

## Technical Overview

* **Output Pin:** `PB1 / OC1A` (DIP-28 Pin 15).
* **Timer Mode:** Fast PWM Mode 14 ($TOP = \text{ICR1}$, `WGM13:10 = 1110`).
* **Prescaler:** 8 (`CS11 = 1`).
* **TOP Value (`ICR1`):** `1999` (2000 counts per PWM period).
* **PWM Frequency:** $16\text{ MHz} / (8 \times 2000) = 1,000\text{ Hz}$ ($1.0\text{ kHz}$).
* **Compare Output Mode:** Non-inverting (`COM1A1 = 1`, `COM1A0 = 0`).
* **Duty Cycle Sequence:** Non-blocking transition through 25%, 50%, and 75% duty cycles every 2.0 seconds.
* **Hardware Autonomy:** Timer1 hardware generates all PWM output edges without CPU or ISR intervention.
