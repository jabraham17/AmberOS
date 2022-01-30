#include "uart.h"
#include <lowlevel/io.h>

void uart_putc(char ch) {
    uart_putc_port(COM1, ch);
}
void uart_puts(char* str) {
 uart_puts_port(COM1, str);
}

void uart_putc_port(word port, char ch) {
    io_outb(port, ch);
}
void uart_puts_port(word port, char* str) {
        while(*str != '\0') {
        uart_putc_port(port, *str);
        str++;
    }
}
