# Laboratory Practices Workspace

This directory contains the standardized laboratory practices for the **ATmega328P Bare-Metal Microcontrollers Course** at **TecNM / Instituto Tecnológico Superior de Jerez**.

---

## Directory Structure

```text
practices/
├── README.md                           # This guide
├── templates/
│   └── PRACTICE_TEMPLATE.md            # Master 20-section authoring template
└── labs/                               # Authored laboratory practices (PR-01 to PR-12)
```

---

## Naming & File Conventions

All authored laboratory practices residing in `practices/labs/` follow the strict naming convention:

`PR-XX_<short-name>.md`

* **Example:** `PR-01_gpio-output.md`
* **Rules:** Lowercase letters, numbers, and hyphens (`-`) only. Spaces are strictly prohibited.

---

## Practice Scaffolding Levels

* **Level A — Guided (PR-01 to PR-04):** Detailed register walkthroughs and reference code provided.
* **Level B — Semi-Guided (PR-05 to PR-08):** Skeleton code provided; students complete register configurations and formulas.
* **Level C — Engineering Challenge (PR-09 to PR-12):** Independent driver and system architecture design.

---

## Toolchain & Execution Commands

Every practice is compiled using the root or local Makefile via PowerShell:

```powershell
# Compile practice firmware
make

# Inspect Flash/SRAM footprint
make size

# Inspect assembly disassembly
make disasm

# Program target ATmega328P via USBasp
make flash

# Clean build artifacts
make clean
```

For complete authoring specifications, see [PRACTICE_AUTHORING_STANDARD.md](../docs/course/PRACTICE_AUTHORING_STANDARD.md).
