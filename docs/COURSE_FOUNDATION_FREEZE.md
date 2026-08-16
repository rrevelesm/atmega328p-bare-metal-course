# Course Foundation Freeze & Infrastructure Sign-Off

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  
**Author:** Rafael Reveles-Martínez  
**Repository:** [rrevelesm/atmega328p-bare-metal-course](https://github.com/rrevelesm/atmega328p-bare-metal-course)  

---

## 1. Infrastructure Status Sign-Off

| Infrastructure Component | Verified Version / Target | Status |
| :--- | :--- | :---: |
| **VS Code Environment** | Portable Workspace (`.vscode/`) | **READY** |
| **AVR Compiler Toolchain** | Microchip AVR GNU Toolchain 4.0.0 (`avr-gcc 15.1.0`) | **READY** |
| **Build System** | xPack GNU Make 4.4.1 (POSIX Makefile pipeline) | **READY** |
| **Hardware Flasher** | Official AVRDUDE 8.2 (Windows x64) | **READY** |
| **Build System Pipeline** | `make`, `make size`, `make disasm` | **READY** |
| **Local CI Definition** | Headless GitHub Actions workflow (`avr-build.yml`) | **READY** |
| **Remote GitHub CI** | GitHub Actions Runner (`ubuntu-latest`) | **PASS** |
| **GitHub Remote Repository**| `rrevelesm/atmega328p-bare-metal-course` | **PUBLIC** |

---

## 2. Pedagogical Foundation Sign-Off

* **Course Architecture:** **FROZEN** (36 Hours, 12 Modules, 12 Practices, Capstone Data Acquisition Station).
* **Learning Outcomes:** **FROZEN** (General Learning Outcome + 10 Specific Outcomes SLO-01 to SLO-10).
* **Practice Engineering Standard:** **FROZEN** (20-Section Master Template & 3-Level Scaffolding Model).
* **PR-01 (Basic GPIO Output):** **AVAILABLE** (Level A Pilot — `firmware/examples/01_gpio_blink/`).
* **PR-02 (GPIO Input & Pull-Up):** **AVAILABLE** (Level A Guided — `firmware/examples/02_gpio_input/`).
* **PR-03 (GPIO Debounce):** **AVAILABLE** (Level A Guided — `firmware/examples/03_gpio_debounce/`).
* **PR-04 (External Interrupts):** **AVAILABLE** (Level A Guided — `firmware/examples/04_external_interrupt/`).
* **PR-05 (Timer0 CTC System Tick):** **AVAILABLE** (Level A Guided — `firmware/examples/05_timer0_ctc/`).
* **PR-06 (Timer1 Hardware PWM):** **AVAILABLE** (Level B Semi-Guided — `firmware/examples/06_timer1_pwm/`).

---

## 3. Physical Hardware Status

* **USBasp Physical Communication:** **PENDING** (Software toolchain prepared; physical programmer attachment pending).
* **Physical Clock & Fuse Bytes:** **PENDING** (Fuses remain unmodified; default internal RC / 16 MHz build assumption documented).

---

## 4. Development Target Sign-Off

NEXT DEVELOPMENT TARGET: PR-07 — ADC
