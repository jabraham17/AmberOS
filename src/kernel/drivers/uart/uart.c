#include "uart.h"
#include <lowlevel/io.h>
#include <stdlib/stdarg.h>
#include <stdlib/string.h>

void uart_putc(char ch) { uart_putc_port(COM1, ch); }
void uart_puts(char* str) { uart_puts_port(COM1, str); }

char printf_buffer[1024];
void uart_printf(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(printf_buffer, fmt, args);
  va_end();
  uart_puts(printf_buffer);
}

void uart_putc_port(word port, char ch) { io_outb(port, ch); }
void uart_puts_port(word port, char* str) {
  while(*str != '\0') {
    uart_putc_port(port, *str);
    str++;
  }
}
