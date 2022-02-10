

override CFLAGS+= -ffreestanding -m32 --target=i386-unknown-linux-gnu
override CFLAGS+= -mno-sse
override LDFLAGS+= 

C_SOURCES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)
C_OBJECTS=$(patsubst %.c,%.o,$(C_SOURCES))
A_SOURCES=$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm)
A_OBJECTS=$(filter-out $(START),$(patsubst %.asm,%_a.o,$(A_SOURCES)))
OBJECTS=$(C_OBJECTS) $(A_OBJECTS)
DEPENDS=$(patsubst %.c,%.d,$(C_SOURCES))
DEPENDS+= $(patsubst %.asm,%_a.d,$(A_SOURCES))

INCLUDE= -I$(ROOT_PROJECT_DIRECTORY)src/kernel

KERNEL_BIN=kernel.o
TARGET=$(BIN_DIRECTORY)$(KERNEL_BIN)

.PHONY: all
all: $(TARGET)
	@:

.PHONY: clean
clean:
	$(RM) $(KERNEL_BIN) $(DEPENDS) $(OBJECTS)

$(TARGET): $(KERNEL_BIN) Makefile
	@cp $(KERNEL_BIN) $(TARGET)

$(KERNEL_BIN): $(OBJECTS) Makefile
	@echo "LD $@"
	@$(LD) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -r -o $@

%.o: %.c Makefile
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(DEPENFLAGS) $(INCLUDE) -c $< -o $@

%_a.o: %.asm Makefile
	@echo "AS $@"
	@$(AS) -felf $(CONSTANTS) $< -o $@ -MD $(patsubst %.asm,%.d,$<)

-include $(DEPENDS)
