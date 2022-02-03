#ifndef _KERNEL_STDLIB_STRING_H_
#define _KERNEL_STDLIB_STRING_H_

#include <stdlib/types.h>

void memcpy(void* dst, void* src, size_t n);
void memmove(void* dst, void* src, size_t n);
void memset(void* dst, uint8_t v, size_t n);

void strncpy(char* dst, char* src, size_t n);
size_t strlen(char* str);
//buf must be size 11
void itoh(int32_t i, char* buf);

#endif
