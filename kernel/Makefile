ARMGNU ?= aarch64-none-elf

COPS = -Wall -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude

BUILD_DIR = build
SRC_DIR = src
BOOT_DIR = booting

all: kernel8.img

clean:
	$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(BUILD_DIR)/*.img $(BUILD_DIR)/kernel8.elf

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(BOOT_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_BOOT_FILES = $(wildcard $(BOOT_DIR)/*.S)
ASM_SRC_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_BOOT_FILES:$(BOOT_DIR)/%.S=$(BUILD_DIR)/%_s.o)
OBJ_FILES += $(ASM_SRC_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(BOOT_DIR)/link.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T $(BOOT_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $(BUILD_DIR)/kernel8.img
