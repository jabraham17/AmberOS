#include "string.h"

void memcpy(void* dst, void* src, size_t n) {
    for(size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
}
void memset(void* dst, uint8_t v, size_t n) {
    for(size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = v;
    }
}
