# Practice Scaffolding & Autonomy Progression Map

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  

---

## Scaffolding Level Definitions

* **Level A (Guided):** High instructor support; full reference code provided; guided register walkthroughs.
* **Level B (Semi-Guided):** Moderate support; code skeletons provided with missing register configuration blocks; guided calculations.
* **Level C (Engineering Challenge):** Independent engineering; problem statement and constraints provided; student designs architecture, drivers, and verification.

---

## Scaffolding Progression Table (PR-01 to PR-12)

| Practice ID | Practice Title | Level | Code Support Provided | Datasheet Autonomy | Measurement Autonomy |
| :--- | :--- | :---: | :--- | :--- | :--- |
| **PR-01** | Basic GPIO Output & LED Blink | **Level A** | Full source code provided | Guided register lookup (`DDRB`/`PORTB`) | Basic LED observation |
| **PR-02** | Push Button Input & Debouncing | **Level A** | Full structure with debouncing algorithm | Guided input register lookup (`PIND`/`PORTD`) | Logic analyzer capture guided |
| **PR-03** | External Interrupts (INT0) & PCINT | **Level A** | Skeleton with vector definitions | Vector table & `EICRA` register lookup | Interrupt response measurement |
| **PR-04** | Non-Blocking System Tick Generator | **Level A** | Full timer CTC skeleton | Prescaler & `OCR0A` formula lookup | Timer tick oscilloscope measurement |
| **PR-05** | Servo & LED Dimmer PWM Control | **Level B** | Partial code; missing `TCCR1A/B` setup | Fast PWM mode & `ICR1` formula lookup | PWM duty cycle & frequency measurement |
| **PR-06** | Analog Sensor Sampling (LM35/Pot) | **Level B** | Partial code; missing ADC init | `ADMUX` channel selection & VREF lookup | Analog voltage calculation vs. measured |
| **PR-07** | Full-Duplex UART Telemetry & `printf` | **Level B** | `fdevopen` skeleton provided | `UBRR0` baud rate formula calculation | Serial terminal baud rate verification |
| **PR-08** | SPI External Memory Interface | **Level B** | SPI transfer function provided | SPI Mode (CPOL/CPHA) & clock lookup | SPI clock & MOSI/MISO trace decoding |
| **PR-09** | I²C / TWI Real-Time Clock / Sensor | **Level C** | Bus state machine specified; no code | `TWBR` bit rate calculation & ACK sequence | I²C packet decoding on logic analyzer |
| **PR-10** | Internal EEPROM Storage | **Level C** | Specification provided; no code | EEAR/EEDR/EECR sequence verification | EEPROM data integrity verification |
| **PR-11** | Watchdog Reset & Low-Power Sleep | **Level C** | Specification provided; no code | WDT prescaler & sleep mode register lookup | Supply current measurement in sleep mode |
| **PR-12** | Bare-Metal Integrator System Station | **Level C** | High-level system requirements only | Full datasheet autonomy across all peripherals | Full system signal verification & telemetry |
