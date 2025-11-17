#include "error.h"
#include "string.h"
#include <drivers/uart/uart.h>

void panic(char* msg) {
  if(msg) {
    uart_puts(msg);
  }
#if defined(DEBUG) && DEBUG == 1
  char buffer[201]; // one extra so stacktrace doesnt overwrite nul char
  memset(buffer, 0, 201);
  stacktrace(buffer, 200, 10);
  uart_printf("\nstack trace %s\n");
#endif
  HALT
}

#if defined(DEBUG) && DEBUG == 1
// Assume, as is often the case, that EBP is the first thing pushed
// If not, we are in trouble.
// this only works in debug mode with no optimizations
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
void stacktrace(char* buffer, size_t nbuffer, size_t maxFrames) {
  struct stackframe* stk;
  __asm__ volatile("mov %0,ebp \n " : "+r"(stk));
  char* bufEnd = buffer + nbuffer;
  for(size_t frame = 0; stk != 0 && frame < maxFrames && buffer < bufEnd;
      frame++) {
    size_t l = sprintf(buffer, "%x\n", (int32_t)stk->eip);
    buffer += l;
    stk = stk->ebp;
  }
}
#endif
