%ifndef _IC_BOOT_ASM_
%define _IC_BOOT_ASM_

bits 16
org 0x7c00 ; correct all labels to correct sector
jmp _boot


%ifndef KERNEL_OFFSET
%error "KERNEL_OFFSET must be defined"
%endif
%ifndef STACK_OFFSET_16
%error "STACK_OFFSET_16 must be defined"
%endif
%ifndef STACK_OFFSET_32
%error "STACK_OFFSET_32 must be defined"
%endif
%ifndef NSECTORS
%error "NSECTORS must be defined"
%endif

%include "disk_load.asm"
%include "gdt.asm"
%include "switch32.asm"
%include "print16.asm"
%include "print32.asm"

bits 16
_boot:
    ; save boot drive
    mov [globals.boot_drive], dl

    mov bp, STACK_OFFSET_16
    mov sp, bp

    call clear_screen_16
    mov ax, strings.real_mode
    call print_str_16

    call load_kernel
    mov ax, strings.load_kernel
    call print_str_16

    mov ax, strings.switch_pm_mode
    call print_str_16

    call switch_32pm

    ; should never get here
    jmp $ ; halt


load_kernel:
    ; load sectors for kernel
    ; cant load too much because of register sizes and disk sizes

    mov bx, KERNEL_OFFSET
    mov dl, [globals.boot_drive]
    mov ax, 0x000f ; num sectrs: BE VERY CAREFUL SETTING THIS VALUE

    call disk_load

    ret


strings:
    .real_mode: db "Booted 16-bit", 0
    .load_kernel: db "Loaded kernel", 0
    .switch_pm_mode: db "Switch 32-bit", 0
    .pm_mode: db "In 32-bit", 0

; global variables
globals:
    .boot_drive: db 0
    .current_screen_row: db 0

; pad and boot signal
times (510-($-$$)) db 0
dw 0xaa55

%endif
