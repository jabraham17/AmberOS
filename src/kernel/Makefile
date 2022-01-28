

override CFLAGS+= -ffreestanding -m32 --target=i386-unknown-linux-gnu
override LDFLAGS+= -Ttext $(KERNEL_OFFSET) --oformat binary

C_SOURCES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)
C_OBJECTS=$(patsubst %.c,%.o,$(C_SOURCES))
START=_start_a.o
A_SOURCES=$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm)
A_OBJECTS=$(filter-out $(START),$(patsubst %.asm,%_a.o,$(A_SOURCES)))
DEPENDS=$(patsubst %.c,%.d,$(C_SOURCES))
DEPENDS+= $(patsubst %.asm,%_a.d,$(A_SOURCES))

TARGET=$(BIN_DIRECTORY)kernel.bin

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(TARGET) $(DEPENDS) $(START) $(C_OBJECTS) $(A_OBJECTS)

$(TARGET): $(START) $(C_OBJECTS) $(A_OBJECTS) Makefile
	$(LD) -Ttext 0x1000 $(START) $(C_OBJECTS) $(A_OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(DEPENFLAGS) $(INCLUDE) -c $< -o $@

%_a.o: %.asm Makefile
	@$(AS) -M $< > $(patsubst %.asm,%_a.d,$<)
	$(AS) -felf $(CONSTANTS) $< -o $@

-include $(DEPENDS)
