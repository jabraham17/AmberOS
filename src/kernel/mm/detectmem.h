#ifndef _KERNEL_MM_DETECTMEM_H_
#define _KERNEL_MM_DETECTMEM_H_

#include "multiboot.h"
#include <stdlib/types.h>

// multiboot vars
extern multiboot_info_t* dm_mbd;
extern uint32_t dm_magic;

void dm_printmm();

#endif
