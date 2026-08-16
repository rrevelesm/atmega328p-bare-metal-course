# PR-01 Pilot Practice Review & Evaluation Report

**Course:** ATmega328P Bare-Metal Microcontrollers  
**Institution:** TecNM / Instituto Tecnológico Superior de Jerez  
**Evaluated Practice:** `PR-01_gpio-output.md` (Basic GPIO Output & LED Control)  
**Scaffolding Level:** `Level A — Guided`  

---

## 1. Executive Summary

Practice `PR-01` was written as the pilot implementation of the 20-section master practice engineering template (`practices/templates/PRACTICE_TEMPLATE.md`). The review confirms that the template provides complete technical rigor, datasheet traceability, and build reproducibility without introducing unnecessary bureaucratic overhead.

---

## 2. Evaluation Criteria Analysis

### A. Template Usability & Completeness
* **20/20 Sections Utilized:** Every section in the master template served a clear purpose. Sections like *Datasheet References*, *Register Map*, and *Disassembly Analysis* effectively bridge the gap between high-level C code and underlying 8-bit AVR hardware.
* **Redundancy Check:** No redundant sections were identified. The distinction between *Software Verification* (offline ELF/HEX/size/LSS checks) and *Physical Verification* (breadboard LED operation) proved essential given the current pending status of USBasp hardware.

### B. Scaffolding Level Appropriateness (Level A — Guided)
* The guided support level for PR-01 is highly appropriate for introductory students. Providing the complete reference code while requiring students to analyze bitwise masks (`|=`, `^=`) and complete the **Datasheet Navigation Task** ensures cognitive engagement without overwhelming beginners.

### C. Estimated Duration (3.0 Hours Total)
* **Allocation:** 1.0 Hour Theory / 2.0 Hours Laboratory Practice.
* **Feasibility:** Verified. The 3.0-hour window is sufficient for students to navigate the datasheet, set up the breadboard circuit, compile using GNU Make, inspect memory footprint and disassembly, and complete the engineering analysis questions.

### D. Key Technical & Pedagogical Insights
1. **ISP Multiplexing Warning:** Explicitly highlighting that pin `PB5` (DIP-28 Pin 19) is shared with the `SCK` programming clock line prevents students from loading down the ISP bus during firmware flashing.
2. **Clock Assumption Clarity:** Clarifying that `F_CPU = 16000000UL` is a build assumption prevents confusion if target microcontrollers run on the default 1 MHz factory internal RC oscillator prior to Fuse configuration.

---

## 3. Recommendations before Authoring PR-02

1. **Keep Master Template Intact:** The 20-section template requires no structural modifications; it is fully ready for authoring `PR-02` (Push Button Input & Debouncing).
2. **Maintain Strict Bare-Metal Rigor:** Continue enforcing explicit register usage (`DDRB`, `PORTB`, `PINB`) without Arduino API wrappers.
3. **Preserve Disassembly Inspection:** Retain `make disasm` step for `PR-02` to show how digital input reading translates into `in` or `sbic`/`sbis` assembly instructions.
