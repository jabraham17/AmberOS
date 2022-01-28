

override CFLAGS+= -ffreestanding -m32 --target=i386-unknown-linux-gnu
override CFLAGS+= -mno-sse
override LDFLAGS+= -Ttext $(KERNEL_OFFSET)

C_SOURCES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)
C_OBJECTS=$(patsubst %.c,%.o,$(C_SOURCES))
START=_start_a.o
A_SOURCES=$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm)
A_OBJECTS=$(filter-out $(START),$(patsubst %.asm,%_a.o,$(A_SOURCES)))
DEPENDS=$(patsubst %.c,%.d,$(C_SOURCES))
DEPENDS+= $(patsubst %.asm,%_a.d,$(A_SOURCES))

TARGET=$(BIN_DIRECTORY)kernel


.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(DEPENDS) $(START) $(C_OBJECTS) $(A_OBJECTS)

.PHONY: $(TARGET)
$(TARGET): $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(START) $(C_OBJECTS) $(A_OBJECTS) Makefile
	$(LD) $(START) $(C_OBJECTS) $(A_OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(DEPENFLAGS) $(INCLUDE) -c $< -o $@

%_a.o: %.asm Makefile
	@$(AS) -M $< > $(patsubst %.asm,%_a.d,$<)
	$(AS) -felf $(CONSTANTS) $< -o $@

-include $(DEPENDS)
