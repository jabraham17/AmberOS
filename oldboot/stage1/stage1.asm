%ifndef _IC_STAGE1_STAGE1_ASM_
%define _IC_STAGE1_STAGE1_ASM_

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

; %include "helper/disk_load.asm"
; %include "helper/print16.asm"
; %include "helper/print32.asm"
; %include "stage1/gdt.asm"
; %include "stage1/switch32.asm"
; %include "stage1/a20.asm"
%include "disk_load.asm"
%include "print16.asm"
%include "print32.asm"
%include "gdt.asm"
%include "switch32.asm"
%include "a20.asm"

bits 16
_boot:

    xor ax, ax ; zero ax, segment registers (not CS) and sp
    mov ds, ax
    mov es, ax
    mov ss, ax
    xor sp, sp

    ; save boot drive
    mov [globals.boot_drive], dl
    mov bp, STACK_OFFSET_16
    mov sp, bp

    call clear_screen_16
    mov ax, strings.real_mode
    call print_str_16

    call enable_a20

    call load_kernel
    mov ax, strings.load_kernel
    call print_str_16

    mov ax, strings.switch_pm_mode
    call print_str_16
    jmp switch_32pm ; we never return from here, this will jump to the kernel

load_kernel:
    ; load sectors for kernel
    ; cant load too much because of register sizes and disk sizes

    xor bx, bx
    mov es, bx ; zero es
    mov ds, bx

    mov bx, KERNEL_OFFSET
    ;mov dl, 0
    mov dl, byte [globals.boot_drive]
    mov ax, 0x001F ; num sectrs: BE VERY CAREFUL SETTING THIS VALUE
    mov cl, (0x02 + STAGE1_NSECTORS)

    call disk_load

    ret


strings:
    .real_mode: db "Booted 16-bit", 0
    .load_kernel: db "Loaded kernel", 0
    .switch_pm_mode: db "Switch 32-bit", 0

; global variables
globals:
    .boot_drive: db 0
    .current_screen_row: db 0

%endif
