# Repository Architecture & Directory Map

This document outlines the organization and purpose of directories and core configuration files within the **ATmega328P Bare-Metal Course** repository.

---

## Directory Map Overview

```text
atmega328p-bare-metal-course/
├── .github/
│   └── workflows/
│       └── avr-build.yml           # GitHub Actions automated headless AVR build pipeline
├── .vscode/
│   ├── c_cpp_properties.json       # Portable C11 IntelliSense & AVR define configuration
│   ├── extensions.json             # Recommended VS Code extension pack recommendations
│   └── tasks.json                  # Portable build tasks (Build, Clean, Size, Disasm, Flash)
├── docs/
│   ├── architecture/               # Hardware, toolchain, build system & CI strategy docs
│   ├── course/                     # Course architecture, outcomes, practice & assessment maps
│   ├── getting-started/            # Student onboarding, Windows setup & troubleshooting guides
│   ├── hardware/                   # Physical USBasp hardware bring-up status reports
│   └── REPOSITORY_MAP.md           # This document
├── firmware/
│   └── examples/
│       ├── 01_gpio_blink/          # PR-01 reference code & Makefile (GPIO Output)
│       └── 02_gpio_input/          # PR-02 reference code & Makefile (GPIO Input & Pull-up)
├── practices/
│   ├── README.md                   # Practice workspace catalog & scaffolding overview
│   ├── templates/
│   │   └── PRACTICE_TEMPLATE.md    # Master 20-section practice authoring template
│   └── labs/                       # Authored student laboratory practice guides (PR-01, PR-02)
├── .gitignore                      # Git exclusion rules (ignores build/, .elf, .hex, .o)
├── CHANGELOG.md                    # Historical record of notable project additions & fixes
├── LICENSE-CODE                    # MIT License for source code, Makefiles & build infrastructure
├── LICENSE-DOCS                    # Creative Commons Attribution 4.0 (CC BY 4.0) for educational docs
├── Makefile                        # Root entry Makefile delegating to example targets
├── Makefile.common                 # Shared AVR compiler flags, target MCU & F_CPU constants
└── README.md                       # Main public project landing page & course summary
```

---

## Core Infrastructure Files

* **`Makefile.common`:** Centralizes shared build configurations (`MCU := atmega328p`, `F_CPU := 16000000UL`, `CFLAGS`, `LDFLAGS`).
* **`Makefile`:** Top-level Makefile supporting `make EXAMPLE_DIR=...` overrides.
* **`LICENSE-CODE`:** MIT License covering all code files, headers, and build scripts.
* **`LICENSE-DOCS`:** CC BY 4.0 License covering all educational practices, markdown reports, and course documentation.
