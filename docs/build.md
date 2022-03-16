# Building

To build, just run `make`. This will create a 

## Dependencies

1. make
2. C11 compiler (I use clang)
3. nasm assembler
4. ld.lld
5. grub-mkrescue

## Configuring

If clang and nasm are installed properly, the only thing you need to configure is the linker. If you have properly installed lld, running `make LD=ld.lld` is sufficient. Otherwise, you will need to specify the full path of the the linker.


