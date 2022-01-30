#include "error.h"
#include "string.h"
#include <drivers/uart/uart.h>


void panic(char* msg) {
    if(msg) {
        uart_puts(msg);
    }
    uart_puts("stacktrace\n");
    char buffer[201]; //one extra so stacktrace doesnt overwrite nul char
    memset(buffer, 0, 201);
    stacktrace(buffer, 200, 10);
    uart_puts(buffer);
    uart_putc('\n');
    HALT
}
//Assume, as is often the case, that EBP is the first thing pushed
//If not, we are in trouble.
struct stackframe {
    struct stackframe* ebp;
    uint32_t eip;
};
void stacktrace(char* buffer, size_t nbuffer, size_t maxFrames) {
    struct stackframe* stk;
    __asm__ volatile ("movl %%ebp,%0 \n " : "+r"(stk));
    *buffer = 'S'; buffer++;
    //Trace("Stack trace:\n");
    for(size_t frame = 0; stk != 0 && frame < maxFrames; frame++) {
        *buffer = 'X'; buffer++;
        // Unwind to previous stack frame
        //Trace("  0x{0:16}     \n", stk->eip);
        stk = stk->ebp;
    }
}

