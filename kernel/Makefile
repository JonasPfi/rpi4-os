TOOLCHAIN_PATH = /usr/share/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin
ASM = aarch64-none-elf-gcc
CXX = aarch64-none-elf-gcc
LINKER = aarch64-none-elf-ld
OBJCPY = aarch64-none-elf-objcopy

SRC_DIR = src
BUILD_DIR = build
BOOTING_DIR = booting
INCLUDE_DIR = include

CFILES = $(wildcard $(SRC_DIR)/*.c)
ASMFILES = $(wildcard $(SRC_DIR)/*.S) $(wildcard $(BOOTING_DIR)/*.S)
OFILES = $(CFILES:.c=.o) $(ASMFILES:.S=.o)
INCLUDES = -I$(INCLUDE_DIR)
GCCFLAGS = -Wall -O0 -ffreestanding -nostdinc -nostdlib -nostartfiles $(INCLUDES)
LDFLAGS = -nostdlib $(INCLUDES)

all: clean $(BUILD_DIR)/kernel8.img

# Create boot.S
$(BUILD_DIR)/boot.o: $(BOOTING_DIR)/boot.S
	$(TOOLCHAIN_PATH)/$(ASM) $(GCCFLAGS) -c $< -o $@

# All C to o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(TOOLCHAIN_PATH)/$(CXX) $(GCCFLAGS) -c $< -o $@

# All S to o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	$(TOOLCHAIN_PATH)/$(ASM) $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(BOOTING_DIR)/%.S
	$(TOOLCHAIN_PATH)/$(ASM) $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
	$(TOOLCHAIN_PATH)/$(LINKER) $(LDFLAGS) $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -T $(BOOTING_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	$(TOOLCHAIN_PATH)/$(OBJCPY) -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

clean:
	rm -rf $(BUILD_DIR)/kernel8.img
	rm -rf $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel8.elf $(OFILES)
