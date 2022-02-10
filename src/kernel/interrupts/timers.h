#ifndef _KERNEL_INTERRUPTS_TIMERS_H_
#define _KERNEL_INTERRUPTS_TIMERS_H_

#include <stdlib/types.h>

uint32_t pit_getTick();
void pit_init(uint16_t frequency);

#endif
