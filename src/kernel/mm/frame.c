#include "frame.h"

extern uint32_t __kernel_start;
extern uint32_t __kernel_start_a;
uint32_t _kernel_start = (uint32_t)&__kernel_start;
extern uint32_t __kernel_end;
uint32_t _kernel_end = (uint32_t)&__kernel_end;

#define _ALIGN_BYTE_OFFSET(_alignment) ((1 << (_alignment)) - 1)

// ALIGN_PTR(_kernel_start, 12)
#define ALIGN_PTR(_ptr, _alignment)                                            \
    ((((uint32_t)_ptr) + _ALIGN_BYTE_OFFSET(_alignment)) &                     \
     ~_ALIGN_BYTE_OFFSET(_alignment))

// ALIGN_PTR(_kernel_start, 0xFFF)
//   #define ALIGN_PTR(_ptr, _mask)\
// ((uint32_t)_ptr + (((~((uint32_t)_ptr)) + 1) & (_mask)))

// align _kernel_end to 4k
void frame_init() {
    SC_printf("kernel start 0x%x\n", _kernel_start);
    SC_printf("kernel end   0x%x\n", _kernel_end);
    SC_printf("kernel start 0x%x\n", &__kernel_start_a);
    SC_printf("kernel start 0x%x\n", ALIGN_PTR(_kernel_start, 12));

    // need to align
}
