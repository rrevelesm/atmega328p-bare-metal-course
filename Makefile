# ==============================================================================
# Root Makefile for ATmega328P Bare-Metal Course
# ==============================================================================

EXAMPLE_DIR ?= firmware/examples/01_gpio_blink

.PHONY: all build size disasm flash clean

all:
	$(MAKE) -C $(EXAMPLE_DIR) all

build:
	$(MAKE) -C $(EXAMPLE_DIR) build

size:
	$(MAKE) -C $(EXAMPLE_DIR) size

disasm:
	$(MAKE) -C $(EXAMPLE_DIR) disasm

flash:
	$(MAKE) -C $(EXAMPLE_DIR) flash

clean:
	$(MAKE) -C $(EXAMPLE_DIR) clean
