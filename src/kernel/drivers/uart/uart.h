#ifndef _KERNEL_DRIVERS_UART_UART_H_
#define _KERNEL_DRIVERS_UART_UART_H_

#include <stdlib/types.h>

#define COM1 0x03F8
#define COM2 0x02F8
#define COM3 0x03E8
#define COM4 0x02E8
#define COM5 0x05F8
#define COM6 0x04F8
#define COM7 0x05E8
#define COM8 0x04E8


void uart_putc(char ch);
void uart_puts(char* str);

void uart_printf(char* fmt, ...);

void uart_putc_port(word port, char ch);
void uart_puts_port(word port, char* str);

#endif
