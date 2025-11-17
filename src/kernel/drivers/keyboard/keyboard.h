#ifndef _KERNEL_DRIVER_KEYBOARD_H_
#define _KERNEL_DRIVER_KEYBOARD_H_

#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64
#define KB_COMMAND_PORT 0x64

void kb_init();

#endif
