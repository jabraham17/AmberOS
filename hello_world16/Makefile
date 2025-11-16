BOOT_SRC=boot.asm
BOOT_BIN=$(patsubst %.asm,%.bin,$(BOOT_SRC))
BOOT_DEPEN=$(filter-out $(BOOT_SRC),$(wildcard *.asm))

DATA_SRC=data.asm
DATA_BIN=$(patsubst %.asm,%.bin,$(DATA_SRC))


TARGET= $(ROOT_PROJECT_DIRECTORY)image.iso

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(wildcard *.bin)

$(TARGET): $(BOOT_BIN) $(DATA_BIN)
	dd if=/dev/zero of=$@ bs=512 count=30
	dd if=$(BOOT_BIN) of=$@ conv=notrunc bs=512 seek=0 count=1
	dd if=$(DATA_BIN) of=$@ conv=notrunc bs=512 seek=1 count=29

$(BOOT_BIN): $(BOOT_DEPEN)

%.bin: %.asm Makefile
	$(AS) $< -o $@
