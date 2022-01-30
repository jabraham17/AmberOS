#ifndef _KERNEL_STDLIB_STRING_H_
#define _KERNEL_STDLIB_STRING_H_

#include <stdlib/types.h>

void memcpy(void* dst, void* src, size_t n);
void memset(void* dst, uint8_t v, size_t n);

#endif
