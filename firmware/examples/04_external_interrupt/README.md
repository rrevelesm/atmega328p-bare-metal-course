# Firmware Example 04: External Interrupt INT0

Bare-metal ATmega328P C firmware example demonstrating asynchronous external hardware interrupt handling using `INT0` on pin `PD2`.

---

## Technical Overview

* **Input Pin:** `PD2 / INT0` (DIP-28 Pin 4) with internal pull-up enabled.
* **Output Pin:** `PB5` (DIP-28 Pin 19) connected to status LED via 330 Ω resistor.
* **Interrupt Sense Control:** `EICRA` configured for falling-edge trigger (`ISC01 = 1`, `ISC00 = 0`).
* **Interrupt Mask:** `EIMSK` bit `INT0` enabled.
* **Global Interrupt:** `sei()` sets `I`-bit in `SREG`.
* **ISR Handler:** `ISR(INT0_vect)` sets `volatile bool button_event` flag and exits immediately.
