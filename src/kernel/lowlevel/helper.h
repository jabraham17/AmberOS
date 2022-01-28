#ifndef _KERNEL_LOWLEVEL_HELPER_H_
#define _KERNEL_LOWLEVEL_HELPER_H_

#define HALT __asm__ volatile ("1: jmp 1b");
#define DEBUG_BREAK __asm__ volatile ("int3");
//__asm__(".Llabel: jmp .Llabel");

void halt();
void debugBreak();

#endif
