
#include <drivers/keyboard/keyboard.h>
#include <drivers/screen/screen.h>

#include <interrupts/idt.h>
#include <interrupts/isr.h>
#include <interrupts/pic.h>
#include <interrupts/timers.h>

#include <stdlib/string.h>

#include <mm/detectmem.h>

void rdmsr(uint32_t code, uint32_t* edx, uint32_t* eax) {
    __asm__ volatile("rdmsr" : "=a"(*eax), "=d"(*edx) : "c"(code) :);
}
void wrmsr(uint32_t code, uint32_t edx, uint32_t eax) {
    __asm__ volatile("wrmsr" : : "c"(code), "a"(eax), "d"(edx) :);
}

void cpuid(uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
    __asm__ volatile("cpuid"
                     : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                     : "a"(*eax), "c"(*ecx)
                     :);
}
void rdtsc(uint32_t* low, uint32_t* high) {
    __asm__ volatile("rdtsc" : "=a"(*low), "=d"(*high));
}

void kernel_main() {

//#define TERM_COLOR VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY
//#define TERM_COLOR VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY
#define TERM_COLOR VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK
    SC_setGlobalAttribute(TERM_COLOR);
    SC_setCursorAttribute(TERM_COLOR);
    SC_clearScreen();

    //kb_init();

    SC_printString("Hello, World!\n");
    SC_printString("I am running in the kernel!!!\n");
    SC_printString("Hello, World!\n");
    SC_printString("I am a really long string that will hopefully wrap around "
                   "on the screen because i am so long");
    SC_printString("Separate\n");
    SC_printString("Lets put some tabs\tin here to see how it gets handled\t");
    SC_printString("now we start a new\tsection\n");

    SC_printf(
        "hello my name is %%%s%% and my favorite number is %d, which in hex is "
        "0x%x",
        "steven", 17, 0x11);

    int big_hex = 0xDEADBEEF;
    SC_printf(
        "\nReally big hex number 0x%hhx, 0x%hx, 0x%x\n", big_hex, big_hex,
        big_hex);

    SC_printf("multiboot ptr 0x%x\nmagic 0x%x\n", dm_mbd, dm_magic);
    dm_printmm();
    frame_init();

#if defined(DEBUG) && DEBUG == 1
    SC_debugColorMatrix();
#endif

    return;
}
