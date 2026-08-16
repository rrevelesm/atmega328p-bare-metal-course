# Practice Authoring Standard & Engineering Guidelines

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  

---

## 1. Overview & Purpose

This document defines the strict engineering and pedagogical standards required for all laboratory practices (PR-01 through PR-12) in the ATmega328P bare-metal course. Every practice must be authored using the master template located at `practices/templates/PRACTICE_TEMPLATE.md`.

---

## 2. Mandatory Section Guidelines

Every authored practice Markdown document must include all 20 standardized sections without omission:

1. **Identification:** Practice ID, title, module, duration, associated SLO, scaffolding level, and difficulty.
2. **Learning Objectives:** 2 to 4 measurable performance verbs.
3. **Technical Prerequisites:** Prior practices and knowledge required.
4. **Required Hardware:** Specific components per team and shared lab equipment.
5. **Required Software:** Standard GNU toolchain configuration (No Arduino IDE).
6. **Datasheet References:** Specific ATmega328P datasheet chapters, registers, bits, and a **Datasheet Navigation Task**.
7. **Engineering Problem Statement:** Realistic engineering scenario driving the lab assignment.
8. **Register Map:** Table mapping registers, addresses, bits used, and functional purpose.
9. **Circuit Schematic & Wiring:** Wiring table matching DIP-28 pins to components.
10. **Firmware Design:** Control algorithm steps and explicit design decisions.
11. **Implementation Code:** Complete or skeleton C code adhering to abstraction rules.
12. **Build Procedure:** Standard Makefile invocation commands (`make`, `make size`).
13. **Programming Procedure:** Standard AVRDUDE `make flash` command with safety warnings.
14. **Verification Procedure:** Step-by-step physical and instrumentation verification instructions.
15. **Expected Observations & Measurement:** Table comparing theoretical vs. measured values.
16. **Technical Engineering Analysis:** Questions requiring datasheet analysis and timing calculations.
17. **Evidence Deliverables Package:** Exact list of files to be submitted by the student.
18. **Evaluation Rubric:** Standardized 100-point grading table.
19. **Common Failure Modes:** Frequently encountered mistakes and solutions.
20. **Cleanup / Reset State:** Teardown and workspace reset commands.

---

## 3. Core Policy Rules

### A. Firmware Abstraction Policy
* **Stage 1 (Practices PR-01 to PR-04):** Direct SFR register manipulation inside `main.c` (e.g., `DDRB |= (1 << DDB5)`). No helper functions.
* **Stage 2 (Practices PR-05 to PR-08):** Inline helper macros and functions introduced while maintaining explicit register names.
* **Stage 3 (Practices PR-09 to PR-11):** Modular drivers encapsulated in separate `.h` and `.c` files (e.g., `uart.h`/`uart.c`). Drivers must document underlying registers and refrain from creating opaque Arduino-like APIs.
* **Stage 4 (Practice PR-12):** Capstone integration using modular drivers in a non-blocking super-loop architecture.
* **Strict Prohibition:** Use of Arduino API functions (`digitalWrite`, `pinMode`, `analogRead`, `delay`, `Serial`, etc.) or custom wrapper libraries mimicking Arduino behavior is strictly forbidden.

### B. Datasheet Navigation Policy
Every technical practice must include a **Datasheet Navigation Task**. The task forces students to locate specific register addresses, bit positions, interrupt vector numbers, or clock prescaler formulas directly within the official Microchip ATmega328P datasheet.

### C. Measurement Policy
For all practices involving physical signals (PWM, timers, UART baud rates, ADC sampling, I2C clock), the practice must mandate a comparative table between:
$$\text{Theoretical Calculated Value} \longleftrightarrow \text{Measured Experimental Value}$$
Tolerances (e.g., $\pm 2\%$) must be evaluated and justified by the student.

### D. Disassembly Inspection Policy
Practices PR-01, PR-03, PR-04, and PR-05 must require students to run `make disasm` and inspect `build/main.lss` to compare C code statements against generated 8-bit AVR assembly instructions (e.g., `sbi`, `cbi`, `in`, `out`, `rjmp`).

---

## 4. Scaffolding Levels (Pedagogical Support)

* **Level A — Guided (PR-01 to PR-04):** Complete or near-complete code provided. Focus is on understanding register mapping, toolchain mechanics, and basic peripheral operations.
* **Level B — Semi-guided (PR-05 to PR-08):** Skeleton code provided with missing initialization blocks and algorithms. Student completes register configurations and calculations.
* **Level C — Engineering Challenge (PR-09 to PR-12):** Problem statement, constraints, and interface requirements provided. Student designs driver architecture, algorithm, and complete firmware independently.
