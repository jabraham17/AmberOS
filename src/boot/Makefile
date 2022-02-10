BOOT_SRC=boot.asm
BOOT_BIN=$(patsubst %.asm,%.o,$(BOOT_SRC))

SOURCES=$(BOOT_SRC)
BINARIES=$(patsubst %.asm,%.o,$(SOURCES))
DEPENDS= $(patsubst %.asm,%.d,$(SOURCES))

TARGET=$(BIN_DIRECTORY)boot.o

.PHONY: all
all: $(TARGET)
	@:

$(TARGET): $(BOOT_BIN) Makefile
	@cp $(BOOT_BIN) $(TARGET)

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(DEPENDS)

%.o: %.asm Makefile
	@echo "AS $@"
	@$(AS) -felf $< -o $@ -MD $(patsubst %.asm,%.d,$<)

-include $(DEPENDS)
