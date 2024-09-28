# Define directories
SRC_DIR := kernel
ARCH_DIR := arch/aarch64-elf
INC_DIR := include

# Source files
CFILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(ARCH_DIR)/*.c)
OFILES := $(CFILES:.c=.o)

# Compiler and flags
GCCFLAGS := -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -I$(INC_DIR)
GCCPATH := /usr/share/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin

# Targets
all: clean kernel8.img

# Assembly source file
$(ARCH_DIR)/boot.o: $(ARCH_DIR)/boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $(ARCH_DIR)/boot.S -o $(ARCH_DIR)/boot.o

# C source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

$(ARCH_DIR)/%.o: $(ARCH_DIR)/%.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

# Link the kernel
kernel8.img: $(ARCH_DIR)/boot.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib $(ARCH_DIR)/boot.o $(OFILES) -T $(ARCH_DIR)/link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

# Clean up
clean:
	/bin/rm -f kernel8.elf $(ARCH_DIR)/*.o $(SRC_DIR)/*.o kernel8.img > /dev/null 2> /dev/null || true
