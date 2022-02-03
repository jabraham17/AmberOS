#include "string.h"

void memcpy(void* dst, void* src, size_t n) {
    for(size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
}
void memmove(void* dst, void* src, size_t n) {
    if(dst < src) {
        // forward copy
        for(size_t i = 0; i < n; i++) {
            ((char*)dst)[i] = ((char*)src)[i];
        }
    } else {
        // backwards copy
        for(; n > 0; n--) {
            ((char*)dst)[n - 1] = ((char*)src)[n - 1];
        }
    }
}
void memset(void* dst, uint8_t v, size_t n) {
    for(size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = v;
    }
}
void strncpy(char* dst, char* src, size_t n) {
    size_t i = 0;
    for(; i < n && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    for(; i < n; i++) {
        dst[i] = '\0';
    }
}

size_t strlen(char* str) {
    size_t i = 0;
    while(*str != '\0') {
        i++;
        str++;
    }
    return i;
}

char getHexDigit(byte b) {
    if(b < 0xA) {
        return b + 0x30;
    } else if(b <= 0xF) {
        return b + (0x41 - 0xA);
    } else {
        return 'X';
    }
}

// buf must be size 11
void itoh(int32_t i, char* buf) {
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = getHexDigit((i >> 28) & 0xF);
    buf[3] = getHexDigit((i >> 24) & 0xF);
    buf[4] = getHexDigit((i >> 20) & 0xF);
    buf[5] = getHexDigit((i >> 16) & 0xF);
    buf[6] = getHexDigit((i >> 12) & 0xF);
    buf[7] = getHexDigit((i >> 8) & 0xF);
    buf[8] = getHexDigit((i >> 4) & 0xF);
    buf[9] = getHexDigit((i >> 0) & 0xF);
    buf[10] = '\0';
}
