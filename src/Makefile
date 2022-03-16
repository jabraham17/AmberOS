TARGET=$(BIN_DIRECTORY)AmberOS
BOOT_BIN=$(BIN_DIRECTORY)boot.o
KERNEL_BIN=$(BIN_DIRECTORY)kernel.o

.PHONY: all
all: $(TARGET)
	@:

$(TARGET): $(TARGET).elf Makefile
	@echo "GRUB $(notdir $@)"
	@$(BUILD_SCRIPT_DIRECTORY)mkBoot.sh $(TARGET) $(BIN_DIRECTORY)

$(TARGET).elf: $(BOOT_BIN) $(KERNEL_BIN) Makefile
	@echo "LD $(notdir $@)"
	@$(LD) -T$(BUILD_SCRIPT_DIRECTORY)link.ld $(BOOT_BIN) $(KERNEL_BIN) -o $@

$(BOOT_BIN): Makefile
	@echo "make --directory=boot"
	@$(MAKE) --directory=boot

$(KERNEL_BIN): Makefile
	@echo "make --directory=kernel"
	@$(MAKE) --directory=kernel
	

.PHONY: clean
clean: 
	$(RM) -r $(TARGET)
	@echo "make --directory=boot clean"
	@$(MAKE) --directory=boot clean
	@echo "make --directory=kernel clean"
	@$(MAKE) --directory=kernel clean

