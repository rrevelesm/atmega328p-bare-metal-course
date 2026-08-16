# ==============================================================================
# Makefile for 02_gpio_input example
# ==============================================================================

include ../../../Makefile.common

TARGET    := main
BUILD_DIR := build
SRCS      := main.c
OBJS      := $(BUILD_DIR)/main.o

.PHONY: all build size disasm flash clean

all: build size disasm

build: $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).lss

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map $(OBJS) -o $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BUILD_DIR)/$(TARGET).lss: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

size: $(BUILD_DIR)/$(TARGET).elf
	@echo "--- FIRMWARE SIZE REPORT ---"
	$(SIZE) --format=berkeley $<

disasm: $(BUILD_DIR)/$(TARGET).lss
	@echo "Disassembly generated at $(BUILD_DIR)/$(TARGET).lss"

flash: $(BUILD_DIR)/$(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PART) -U flash:w:$<:i

clean:
	rm -rf $(BUILD_DIR)
