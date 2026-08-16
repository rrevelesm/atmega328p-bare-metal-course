# Practice {{PRACTICE_ID}}: {{PRACTICE_TITLE}}

---

## 1. Identification

* **Practice ID:** `{{PRACTICE_ID}}`
* **Practice Title:** {{PRACTICE_TITLE}}
* **Module:** {{MODULE_ID}} — {{MODULE_NAME}}
* **Estimated Duration:** {{ESTIMATED_DURATION}} Hours
* **Associated SLO:** {{ASSOCIATED_SLO}}
* **Scaffolding Level:** {{SCAFFOLDING_LEVEL}} (Guided / Semi-guided / Engineering challenge)
* **Difficulty:** {{DIFFICULTY}} (Introductory / Intermediate / Advanced)

---

## 2. Learning Objectives

By completing this laboratory practice, the student will be able to:
1. {{OBJECTIVE_1}}
2. {{OBJECTIVE_2}}
3. {{OBJECTIVE_3}}

---

## 3. Technical Prerequisites

* **Prerequisite Practices:** {{PREREQUISITE_PRACTICES}}
* **Required Knowledge:** {{REQUIRED_KNOWLEDGE}}

---

## 4. Required Hardware

### Components per Student / Team
| Component / Part | Quantity | Description / Specification |
| :--- | :---: | :--- |
| {{COMPONENT_1}} | {{QTY_1}} | {{SPEC_1}} |

### Shared Laboratory Equipment
* {{SHARED_EQUIPMENT_1}}

---

## 5. Required Software & Toolchain

* **Editor:** VS Code
* **Compiler:** `avr-gcc` (C11 standard)
* **Build System:** GNU Make
* **Flasher:** AVRDUDE (v8.x+)
* **Framework Policy:** **Strictly Bare-Metal C** (No Arduino IDE, Wiring libraries, or `digitalWrite`/`analogRead` functions allowed).

---

## 6. Datasheet References (Microchip ATmega328P)

Students must consult the official ATmega328P datasheet (DS40002061) for the following sections:

| Peripheral / Topic | Datasheet Section | Relevant Registers | Key Bits / Flags |
| :--- | :--- | :--- | :--- |
| {{PERIPHERAL_1}} | {{SECTION_1}} | {{REGISTERS_1}} | {{BITS_1}} |

> **Datasheet Navigation Task:**  
> {{DATASHEET_TASK_DESCRIPTION}}

---

## 7. Engineering Problem Statement

{{ENGINEERING_PROBLEM_STATEMENT}}

---

## 8. Register Map

| Register | Address | Bits Used | Function in This Practice |
| :--- | :---: | :--- | :--- |
| `{{REGISTER_1}}` | `{{ADDR_1}}` | `{{BITS_1}}` | {{FUNCTION_1}} |

---

## 9. Circuit Schematic & Wiring Table

| ATmega328P Pin | Physical Pin (DIP-28) | External Component | Connection Details |
| :--- | :---: | :--- | :--- |
| `{{PIN_1}}` | `{{DIP_PIN_1}}` | {{COMPONENT_NAME_1}} | {{WIRING_DETAILS_1}} |

---

## 10. Firmware Design & Algorithm

### Control Flow Algorithm
1. {{ALGORITHM_STEP_1}}
2. {{ALGORITHM_STEP_2}}
3. {{ALGORITHM_STEP_3}}

### Design Decisions
* {{DESIGN_DECISION_1}}

---

## 11. Implementation Code

```c
/* 
 * Practice: {{PRACTICE_ID}} - {{PRACTICE_TITLE}}
 * Target MCU: ATmega328P
 * Clock: F_CPU = 16000000UL
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /* Step 1: Peripheral Register Initialization */
    {{INITIALIZATION_CODE}}

    /* Step 2: Main Event Loop */
    while (1) {
        {{MAIN_LOOP_CODE}}
    }

    return 0;
}
```

---

## 12. Build Procedure

Execute the compilation workflow from the root or practice directory using PowerShell:

```powershell
# 1. Clean workspace
make clean

# 2. Compile firmware and generate ELF, HEX, MAP, LSS
make

# 3. Inspect binary memory footprint
make size

# 4. (Optional) Inspect assembly disassembly
make disasm
```

---

## 13. Programming Procedure (ISP Flashing)

> **Hardware Warning:** Ensure USBasp programmer is connected to target ISP header before flashing.

```powershell
# Flash firmware image to target ATmega328P Flash memory
make flash
```

---

## 14. Verification Procedure

1. **Visual / Electrical Verification:** {{VERIFICATION_STEP_1}}
2. **Instrumentation Measurement:** {{VERIFICATION_STEP_2}}

---

## 15. Expected Observations & Experimental Measurement

| Parameter / Signal | Theoretical Value | Measured Value | Tolerance / Error |
| :--- | :---: | :---: | :---: |
| {{PARAM_1}} | {{THEORETICAL_1}} | {{MEASURED_1}} | {{TOLERANCE_1}} |

---

## 16. Technical Engineering Analysis

Answer the following engineering questions based on datasheet analysis and experimental measurements:
1. {{ANALYSIS_QUESTION_1}}
2. {{ANALYSIS_QUESTION_2}}
3. {{ANALYSIS_QUESTION_3}}

---

## 17. Evidence Deliverables Package

Submit a compressed ZIP or GitHub PR containing:
1. Complete source code (`main.c`, Makefile).
2. Clean compilation output and `make size` log.
3. Oscilloscope / Logic Analyzer waveform capture or terminal log file.
4. Completed technical analysis answers in report Markdown format.

---

## 18. Evaluation Rubric (100 Points Total)

| Rubric Criterion | Description | Points |
| :--- | :--- | :---: |
| **Technical Functionality** | Code executes expected hardware control correctly without locks or bugs. | 30 |
| **Register Control Rigor** | Explicit SFR register configuration without Arduino API functions or magic numbers. | 25 |
| **Code Quality & Build System** | Strict C11, clean compilation with zero warnings, correct Makefile targets. | 15 |
| **Engineering & Datasheet Analysis** | Correct answers to datasheet navigation task and analysis questions. | 15 |
| **Experimental Measurement & Evidence** | Accurate waveform captures, timing measurements, and complete deliverables. | 15 |
| **Total** | | **100** |

---

## 19. Common Failure Modes & Troubleshooting

* **Issue 1:** {{FAILURE_MODE_1}} -> *Solution:* {{SOLUTION_1}}
* **Issue 2:** {{FAILURE_MODE_2}} -> *Solution:* {{SOLUTION_2}}

---

## 20. Workspace Cleanup & Reset State

```powershell
make clean
```
Ensure breadboard power is disconnected before proceeding to the next practice.
