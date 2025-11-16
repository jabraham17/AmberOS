
#ifndef _KERNEL_STDLIB_ERROR_H_
#define _KERNEL_STDLIB_ERROR_H_

#include "types.h"

#define HALT __asm__ volatile (".label: jmp .label");
void panic(char* msg);
void stacktrace(char* buffer, size_t nbuffer, size_t maxFrames);

#endif

