# Assessment Strategy & Grading Policy

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  

---

## 1. Grading Scheme (100% Total)

| Component | Weight (%) | Description |
| :--- | :---: | :--- |
| **Laboratory Practices (12 Practices)** | **40%** | Individual/pair completion of practices PR-01 to PR-11, code correctness, and short technical verification logs. |
| **Technical Quizzes & Datasheet Audits** | **20%** | Short evaluations on register mapping, bitwise operations, timer prescaler calculations, and datasheet reading. |
| **Capstone Integrator Project (PR-12)** | **30%** | Multi-peripheral firmware integration, physical hardware execution, measurement evidence, and technical presentation. |
| **Firmware Engineering & Build Quality** | **10%** | Adherence to engineering rules (no magic numbers, strict `stdint.h`, correct Makefiles, zero compiler warnings). |
| **Total** | **100%** | |

---

## 2. Evaluation Rubric Criteria

### A. Laboratory Practices (40%)
* **Functionality (50% of practice grade):** Firmware compiles cleanly without warnings and executes desired hardware control on ATmega328P.
* **Bare-Metal Register Rigor (30% of practice grade):** Code uses explicit SFR register names and bitwise operations without Arduino API functions or opaque wrappers.
* **Verification Evidence (20% of practice grade):** Submission of verified memory footprint (`avr-size`), disassembly snippet (`main.lss`), and logic analyzer/terminal logs.

### B. Capstone Integrator Project (30%)
* **Peripheral Integration (30%):** Concurrent operation of GPIO, Timers, ADC, UART, and I²C/SPI using a non-blocking super-loop architecture.
* **Firmware Robustness (30%):** Atomic sections for shared variables, Watchdog protection, and error handling.
* **Hardware Demonstration (20%):** Physical operation on breadboard/PCB with signal verification.
* **Technical Documentation (20%):** Architecture block diagram, memory map analysis, and clean Git repository commit history.
