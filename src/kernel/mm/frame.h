#ifndef _KERNEL_MM_FRAME_H_
#define _KERNEL_MM_FRAME_H_

#include <stdlib/types.h>

// statically decide max number of frames
// TODO: make dynamic
#define NFRAMES 1024

extern uint32_t frames[NFRAMES / 32];
#define SET_FRAME_BIT(_idx)                                                    \
    frames[(uint32_t)((_idx) / 32)] |= (0x01 << (uint32_t)((_idx) % 32))
#define CLEAR_FRAME_BIT(_idx)                                                  \
    frames[(uint32_t)((_idx) / 32)] &= ~(0x01 << (uint32_t)((_idx) % 32))
#define GET_FRAME_BIT(_idx)                                                    \
    (frames[(uint32_t)((_idx) / 32)] & (0x01 << (uint32_t)((_idx) % 32)))


extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;

#define _ALIGN_BYTE_OFFSET(_alignment) ((1 << (_alignment)) - 1)

// ALIGN_PTR(_kernel_start, 12)
#define ALIGN_PTR(_ptr, _alignment)                                            \
    ((((uintptr_t)_ptr) + _ALIGN_BYTE_OFFSET(_alignment)) &                     \
     ~_ALIGN_BYTE_OFFSET(_alignment))

// ALIGN_PTR(_kernel_start, 0xFFF)
//   #define ALIGN_PTR(_ptr, _mask)\
// ((uintptr_t)_ptr + (((~((uintptr_t)_ptr)) + 1) & (_mask)))


void frame_init();

#endif
