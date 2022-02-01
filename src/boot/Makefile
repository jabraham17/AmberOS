MBR_SRC=mbr/mbr.asm
STAGE1_SRC=stage1/stage1.asm

MBR_BIN=$(patsubst %.asm,%.bin,$(MBR_SRC))
STAGE1_BIN=$(patsubst %.asm,%.bin,$(STAGE1_SRC))

SOURCES=$(MBR_SRC) $(STAGE1_SRC)
BINARIES=$(patsubst %.asm,%.bin,$(SOURCES))
DEPENDS= $(patsubst %.asm,%.d,$(SOURCES))


TARGET=$(BIN_DIRECTORY)boot.bin

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(DEPENDS)

$(TARGET): $(BINARIES) Makefile
	cat $(BINARIES) > $@

# building the mbr requires first building the stage1 so we know the size
$(MBR_BIN): $(STAGE1_BIN)

# double excape shell so it gets run later
override CONSTANTS+= -dSTAGE1_NSECTORS=$$($(BUILD_SCRIPT_DIRECTORY)countSectors.sh $(STAGE1_BIN))

# generate depencies
# assembly to binary
# if not an even 512 byte multiple, make it so
%.bin: %.asm Makefile
	@$(AS) -M $< > $(patsubst %.asm,%.d,$<)
	$(AS) -fbin $(CONSTANTS) $< -o $@
	$(BUILD_SCRIPT_DIRECTORY)pad.sh $@


-include $(DEPENDS)
