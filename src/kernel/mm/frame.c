#include "frame.h"


// align _kernel_end to 4k
void frame_init() {
    SC_printf("kernel start  0x%x\n", _kernel_start);
    SC_printf("kernel end    0x%x\n", _kernel_end);
    SC_printf("kernel astart 0x%x\n", ALIGN_PTR(_kernel_start, 12));
    SC_printf("kernel aend   0x%x\n", ALIGN_PTR(_kernel_end, 12));

    // need to align
}
