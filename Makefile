# =========================================================
# Cortex-A Bare-metal Makefile (QEMU virt)
# =========================================================

# Toolchain
CROSS_COMPILE ?= arm-none-eabi-
CC      := $(CROSS_COMPILE)gcc
LD      := $(CROSS_COMPILE)ld
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy

# Project
TARGET      := kernel
BUILD_DIR  := build
LINKER     := linker/virt.ld

# CPU / Architecture
ARCH_FLAGS := -mcpu=cortex-a15 -marm

# Compiler flags
CFLAGS  := $(ARCH_FLAGS)
CFLAGS  += -ffreestanding -nostdlib -nostartfiles
CFLAGS  += -Wall -Wextra -O0 -g
CFLAGS  += -Iinclude -Isrc

# Linker flags
LDFLAGS := -T $(LINKER) -nostdlib

# Sources
SRCS := \
    src/start.S \
    src/main.c \
    src/uart_pl011.c

# Objects
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
OBJS := $(OBJS:.S=.o)

# =========================================================
# Rules
# =========================================================

.PHONY: all clean run dump

all: $(BUILD_DIR)/$(TARGET).elf

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src

# Compile C
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly
$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Optional binary image
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Run on QEMU
run: $(BUILD_DIR)/$(TARGET).elf
	qemu-system-arm \
		-M virt \
		-cpu cortex-a15 \
		-nographic \
		-kernel $<

# Disassembly (very useful for scheduler debugging)
dump: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -D $< > $(BUILD_DIR)/$(TARGET).lst

# Clean
clean:
	rm -rf $(BUILD_DIR)
