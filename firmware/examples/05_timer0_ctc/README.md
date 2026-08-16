# Firmware Example 05: Timer0 CTC & 1 ms System Tick

Bare-metal ATmega328P C firmware example demonstrating hardware timing using **Timer/Counter0 in CTC mode** to generate a 1 ms system timebase tick.

---

## Technical Overview

* **MCU Clock:** Assumed `F_CPU = 16000000UL` (16 MHz).
* **Timer Mode:** Clear Timer on Compare Match (CTC Mode, `WGM01 = 1`).
* **Prescaler:** 64 (`CS01 = 1`, `CS00 = 1`).
* **Timer Counter Max:** `OCR0A = 249` (250 ticks per interrupt).
* **Interrupt Frequency:** $16\text{ MHz} / (64 \times 250) = 1,000\text{ Hz}$ ($T = 1\text{ ms}$).
* **SRAM Timebase:** 32-bit `volatile uint32_t system_ticks_ms` accessed via `<util/atomic.h>` `ATOMIC_BLOCK`.
* **Output Pin:** `PB5` (DIP-28 Pin 19) toggling every 500 ms without blocking `_delay_ms()`.
