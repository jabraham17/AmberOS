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

// char getHexDigit(byte b) {
//     if(b < 0xA) {
//         return b + 0x30;
//     } else if(b <= 0xF) {
//         return b + (0x41 - 0xA);
//     } else {
//         return 'X';
//     }
// }

// // buf must be size 11
// void itoh(int32_t i, char* buf) {
//     buf[0] = '0';
//     buf[1] = 'x';
//     buf[2] = getHexDigit((i >> 28) & 0xF);
//     buf[3] = getHexDigit((i >> 24) & 0xF);
//     buf[4] = getHexDigit((i >> 20) & 0xF);
//     buf[5] = getHexDigit((i >> 16) & 0xF);
//     buf[6] = getHexDigit((i >> 12) & 0xF);
//     buf[7] = getHexDigit((i >> 8) & 0xF);
//     buf[8] = getHexDigit((i >> 4) & 0xF);
//     buf[9] = getHexDigit((i >> 0) & 0xF);
//     buf[10] = '\0';
// }

void strnreverse(char* str, size_t length) {
    int start = 0;
    int end = length - 1;
    while(start < end) {
        // swap
        char t = str[end];
        str[end] = str[start];
        str[start] = t;

        start++;
        end--;
    }
}

// buf must be at 11
size_t itoa(int num, char* buf, int base) {
    size_t i = 0;

    if(num == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return i;
    }
    bool neg = num < 0 && base == 10;
    unsigned val = neg ? -num : (unsigned)num;

    while(val != 0) {
        int rem = val % base;
        buf[i++] = (rem < 10) ? rem + '0' : rem + 'A' - 10;
        val /= base;
    }

    if(neg)
        buf[i++] = '-';
    buf[i] = '\0';

    // printed it backwards, reverse buf
    strnreverse(buf, i);

    return i;
}

// UNSAFE
size_t vsprintf(char* buffer, char* fmt, va_list args) {

    char* bufStart = buffer;
    size_t size_mask = 0xFFFFFFFF;

    // d for decimal, x for hex, c for char, s for shr
    while(*fmt != '\0') {
        if(*fmt == '%') {
            fmt++;

            // size quailifer
            switch(*fmt) {
                case 'h':
                    size_mask = 0xFFFF;
                    fmt++;
                    if(*fmt == 'h') {
                        size_mask = 0xFF;
                        fmt++;
                    }
                    break;
                default:
                    size_mask = 0xFFFFFFFF;
            }

            switch(*fmt) {
                case '%':
                    *buffer++ = '%';
                    fmt++;
                    break;
                case 'c': {
                    char c = (char)(va_arg(args, int));
                    *buffer++ = c;
                    fmt++;
                    break;
                }
                case 's': {
                    char* s = (char*)va_arg(args, int);
                    while(*s != '\0')
                        *buffer++ = *s++;
                    fmt++;
                    break;
                }
                case 'b': {
                    int b = (va_arg(args, int)) & size_mask;
                    size_t l = itoa(b, buffer, 2);
                    buffer += l; // move buffer after
                    fmt++;
                    break;
                }
                case 'x': {
                    int x = (va_arg(args, int)) & size_mask;
                    size_t l = itoa(x, buffer, 16);
                    buffer += l; // move buffer after
                    fmt++;
                    break;
                }
                case 'd': {
                    int d = (va_arg(args, int)) & size_mask;
                    size_t l = itoa(d, buffer, 10);
                    buffer += l; // move buffer after
                    fmt++;
                    break;
                }
            }
        } else {
            *buffer++ = *fmt++;
        }
    }
    *buffer = '\0';
    return (size_t)(buffer - bufStart);
}

// UNSAFE
size_t sprintf(char* buffer, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int l = vsprintf(buffer, fmt, args);
    va_end();
    return l;
}
