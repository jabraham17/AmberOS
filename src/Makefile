export NSECTORS=2048
export KERNEL_OFFSET=0x1000
export CONSTANTS=-dKERNEL_OFFSET=$(KERNEL_OFFSET) -dSTACK_OFFSET_16=0x9000 -dSTACK_OFFSET_32=0x90000 -dNSECTORS=$(NSECTORS)

TARGET=$(BIN_DIRECTORY)image.iso
BOOT_BIN=$(BIN_DIRECTORY)boot.bin
KERNEL_BIN=$(BIN_DIRECTORY)kernel.bin

.PHONY: all
all: $(TARGET)
	@:

$(TARGET): $(BOOT_BIN) $(KERNEL_BIN) Makefile
	dd if=/dev/zero of=$@ bs=512 count=$(NSECTORS)
	dd if=$(BOOT_BIN) of=$@ conv=notrunc bs=512 seek=0 count=1
	dd if=$(KERNEL_BIN) of=$@ conv=notrunc bs=512 seek=1 count=1

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

