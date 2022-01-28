#include "string.h"
#include "../lowlevel/helper.h"

void memcpy(void* dst, void* src, unsigned long long n) {
    for(unsigned long long i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
}
void memset(void* dst, unsigned char v, unsigned long long n) {
    for(unsigned long long i = 0; i < n; i++) {
        ((char*)dst)[i] = v;
    }
}
