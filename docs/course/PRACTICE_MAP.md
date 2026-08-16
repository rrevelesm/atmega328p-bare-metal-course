# Practice Map: ATmega328P Bare-Metal Course

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  

---

## Practice Catalog (12 Laboratory Practices)

| Practice ID | Practice Name | MCU / Peripheral Target | Specific Learning Outcome | Deliverable Evidence |
| :--- | :--- | :--- | :--- | :--- |
| **PR-01** | Basic GPIO Output & LED Blink | PORTB (`DDRB`, `PORTB`, `PB5`) | **SLO-01** | Compilable C code, HEX binary, size report. |
| **PR-02** | Push Button Input & Debouncing | PORTD / PORTB (`PIND`, `PORTD`) | **SLO-02** | Source code, logic analyzer capture of debounced signal. |
| **PR-03** | External Interrupts (INT0) & PCINT | `EICRA`, `EIMSK`, `PCICR`, `ISR()` | **SLO-03** | Source code, interrupt latency verification report. |
| **PR-04** | Non-Blocking System Tick Generator | Timer0 / Timer2 (CTC Mode) | **SLO-04** | Source code, LED timing measurements on oscilloscope. |
| **PR-05** | Servo & LED Dimmer PWM Control | Timer1 (16-bit Fast PWM, `OCR1A`) | **SLO-05** | Source code, duty cycle waveform capture. |
| **PR-06** | Analog Sensor Sampling (LM35/Pot) | ADC (`ADMUX`, `ADCSRA`, `ADCW`) | **SLO-06** | Source code, sensor voltage conversion tables. |
| **PR-07** | Full-Duplex UART Telemetry & `printf` | USART0 (`UBRR0`, `UCSR0`, `fdevopen`) | **SLO-07** | Source code, serial terminal log file. |
| **PR-08** | SPI External Memory Interface | SPI (`SPCR`, `SPSR`, `SPDR`) | **SLO-08** | Source code, SPI bus transaction capture. |
| **PR-09** | I²C / TWI Real-Time Clock / Sensor | TWI (`TWBR`, `TWSR`, `TWDR`, `TWCR`) | **SLO-09** | Source code, I²C packet decode log. |
| **PR-10** | Internal EEPROM Configuration Storage | EEPROM (`<avr/eeprom.h>`) | **SLO-10** | Source code, persistent reboot log verification. |
| **PR-11** | Watchdog Reset & Low-Power Sleep | WDT & Power (`<avr/wdt.h>`, `<avr/sleep.h>`) | **SLO-10** | Source code, supply current measurement log. |
| **PR-12** | Bare-Metal Integrator System Station | All Core Peripherals Integrated | **SLO-01 to SLO-10** | Complete firmware repository, capstone report & demo. |
