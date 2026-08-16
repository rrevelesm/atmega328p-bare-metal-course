# Learning Outcomes & Student Profile

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  

---

## 1. Student Entry Profile

### Mandatory Prerequisites (Before Course)
* Basic C programming syntax: variables, control structures (`if`, `for`, `while`), functions, pointers, and bitwise operators (`&`, `|`, `~`, `^`, `<<`, `>>`).
* Binary and Hexadecimal number systems, masks, and truth tables.
* Fundamentals of electrical circuit analysis: Ohm's law, pull-up/pull-down resistors, LED current limiting.
* Basic breadboard wiring and digital multimeter usage.

### Acquired During Course
* Reading and extracting timing/register information from official Microchip/Atmel datasheets.
* Direct manipulation of Special Function Registers (SFRs) using `avr-libc` header files (`<avr/io.h>`).
* AVR GNU cross-compiler toolchain compilation (`avr-gcc`), Makefile construction, and ISP programming with AVRDUDE.
* Asynchronous hardware event handling using Interrupt Service Routines (`ISR`).
* Configuration of core peripherals: GPIO, Timers/Counters, PWM, ADC, UART, SPI, I²C/TWI, EEPROM, Watchdog.

### Desirable (Optional)
* Logic analyzer / oscilloscope waveform analysis.
* Basic schematic reading in KiCad / Fritzing.

---

## 2. General Learning Outcome (GLO)

Upon successful completion of the course, the student will be capable of **interpreting official microcontroller datasheets**, **configuring hardware peripheral registers in C bare-metal**, **building reproducible cross-compiled firmware images with GNU AVR toolchains**, **flashing microcontrollers via ISP interfaces**, and **verifying real-time signal timing and communication protocols using laboratory instrumentation** to engineer robust embedded systems.

---

## 3. Specific Learning Outcomes (SLO)

* **SLO-01 (GPIO Output & Bitwise):** Configure digital output pins using `DDRx` and `PORTx` registers and bitwise masking (`_BV()`, `|=`, `&= ~`, `^=`) to control external loads.
* **SLO-02 (GPIO Input & Debounce):** Read digital input states using `PINx` registers with internal pull-up resistors enabled and implement software anti-rebote (debouncing) algorithms.
* **SLO-03 (Interrupts):** Configure external interrupt lines (`INT0`, `INT1`) and Pin Change Interrupts (`PCINT`) to handle asynchronous hardware events with safe atomic sections.
* **SLO-04 (Timers/Counters):** Configure Timer0/Timer2 in Normal and CTC modes to generate deterministic, non-blocking system time ticks.
* **SLO-05 (PWM Generation):** Configure 16-bit Timer1 in Fast PWM and Phase Correct PWM modes to control signal duty cycle for power and actuator positioning.
* **SLO-06 (ADC Acquisition):** Configure the 10-bit Analog-to-Digital Converter, selecting voltage references, channel multiplexing, and prescalers for sensor sampling.
* **SLO-07 (UART Communication):** Calculate baud rate registers (`UBRR0`), format data frames, and implement interrupt-driven serial transmission/reception with custom `stdio` (`printf`) redirection.
* **SLO-08 (SPI Protocol):** Configure the SPI peripheral in Master/Slave modes to transmit and receive full-duplex data with external devices.
* **SLO-09 (I²C / TWI Protocol):** Implement I²C Master communication managing START, STOP, ACK/NACK conditions and bit rate register (`TWBR`) calculations.
* **SLO-10 (EEPROM & Power Management):** Read/write persistent data to internal EEPROM safely using `<avr/eeprom.h>`, configure the Watchdog Timer (`WDT`), and implement sleep modes.
