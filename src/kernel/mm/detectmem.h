#ifndef _KERNEL_MM_DETECTMEM_H_
#define _KERNEL_MM_DETECTMEM_H_

#include <stdlib/types.h>
#include "multiboot.h"

//multiboot vars
extern multiboot_info_t* dm_mbd;
extern uint32_t dm_magic;

void dm_printmm();

#endif
