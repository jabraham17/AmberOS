BOOT_SRC=boot.asm
DEPENDS=$(patsubst %.asm,%.d,$(BOOT_SRC))

TARGET=$(BIN_DIRECTORY)boot.bin

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(DEPENDS)

$(TARGET): $(BOOT_SRC) Makefile
	@$(AS) -M $< > $(patsubst %.asm,%.d,$<)
	$(AS) -fbin $(CONSTANTS) $< -o $@

-include $(DEPENDS)
