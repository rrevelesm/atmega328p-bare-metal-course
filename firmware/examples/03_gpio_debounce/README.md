# Firmware Example 03: GPIO Debounce

Bare-metal ATmega328P C firmware example demonstrating mechanical switch contact debouncing using a state machine architecture.

---

## Technical Overview

* **Input Pin:** `PD2` (DIP-28 Pin 4) with internal pull-up enabled.
* **Output Pin:** `PB5` (DIP-28 Pin 19) connected to status LED via 330 Ω resistor.
* **Debounce Strategy:** Non-blocking Finite State Machine (FSM) filtering contact bounce across a 20 ms stability threshold (`DEBOUNCE_MS`).
* **Event Trigger:** LED state toggles ONCE per stable button press event.
