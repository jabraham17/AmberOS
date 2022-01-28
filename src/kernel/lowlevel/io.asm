%ifndef _IC_KERNEL_LOWLEVEL_IO_ASM_
%define _IC_KERNEL_LOWLEVEL_IO_ASM_

global io_inb
global io_outb
global io_inw
global io_outw

io_inb:
    movzx edx, word [esp+4] ; port
    in al, dx
    ret

io_outb:
    movzx edx, word [esp+4] ; port
    mov al, [esp+8] ; data
    out dx, al
    ret

io_inw:
    movzx edx, word [esp+4] ; port
    in ax, dx
    ret

io_outw:
    movzx edx, word [esp+4] ; port
    mov ax, [esp+8] ; data
    out dx, ax
    ret


%endif
