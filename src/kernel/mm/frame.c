#include "frame.h"
#include <drivers/screen/screen.h>

uint32_t frames[NFRAMES / 32];

extern uint32_t __kernel_start;
uintptr_t _kernel_start = (uintptr_t)&__kernel_start;
extern uint32_t __kernel_end;
uintptr_t _kernel_end = (uintptr_t)&__kernel_end;

// align _kernel_end to 4k
void frame_init() {
  SC_printf("kernel start  0x%x\n", _kernel_start);
  SC_printf("kernel end    0x%x\n", _kernel_end);
  SC_printf("kernel astart 0x%x\n", ALIGN_PTR(_kernel_start, 12));
  SC_printf("kernel aend   0x%x\n", ALIGN_PTR(_kernel_end, 12));

  // need to align
}
