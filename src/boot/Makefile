BOOT_SRC=boot.asm
DEPENDS=$(patsubst %.asm,%.d,$(BOOT_SRC))
# BOOT_DEPEN=$(filter-out $(BOOT_SRC),$(wildcard *.asm))

TARGET=$(BIN_DIRECTORY)boot.bin

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(DEPENDS)

%.bin: %.asm Makefile
	@$(AS) -M $< -o $(patsubst %.asm,%.d,$<)
	$(AS) -fbin $(CONSTANTS) $< -o $@

-include $(DEPENDS)
