SOURCES=$(wildcard *.asm)
BINARIES=$(patsubst %.asm,%.o,$(SOURCES))
DEPENDS= $(patsubst %.asm,%.d,$(SOURCES))

BOOT_BIN=boot.o

TARGET=$(BIN_DIRECTORY)$(BOOT_BIN)

.PHONY: all
all: $(TARGET)
	@:

$(TARGET): $(BOOT_BIN) Makefile
	@cp $(BOOT_BIN) $(TARGET)

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(DEPENDS)

$(BOOT_BIN): $(BINARIES)
	@echo "LD $@"
	@$(LD) $(BINARIES) $(LDFLAGS) $(LDLIBS) -r -o $@

%.o: %.asm Makefile
	@echo "AS $@"
	@$(AS) -felf $< -o $@ -MD $(patsubst %.asm,%.d,$<)

-include $(DEPENDS)
