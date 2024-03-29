%ifndef _IC_STAGE1_GDT_ASM_
%define _IC_STAGE1_GDT_ASM_

global gdt_init
gdt_init:
    cli ; disable interrupts
    lgdt [gdt_def.gdt_descriptor] ; load descriptor

    mov ax, DATA_SEG ; reset all regment regs to data 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp CODE_SEG:.flush  ; far jump to 32 bit code
.flush:
    ret

gdt_def:
.gdt_start:

.gdt_null: ; mandatory null desc, 8 bytes of zero
dd 0x0
dd 0x0

.gdt_code: ; code segment descriptor
; base=0x0, limit=0xfffff,
; 1st flags: (present)1 (privilege)00 (descriptor type)1 -> 1001b
; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
; 2nd flags: granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
dw 0xffff ; Limit (bits 0-15)
dw 0x0 ; Base (bits 0-15)
db 0x0 ; Base (bits 16-23)
db 10011010b ; 1st flags, type flags
db 11001111b ; 2nd flags, Limit (bits 16-19)
db 0x0 ;  Base (bits 24-31)

.gdt_data: ; data segement descriptor
; Same as code segment except for the type flags:
; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
dw 0xffff ; Limit (bits 0-15)
dw 0x0 ; Base (bits 0-15)
db 0x0 ; Base (bits 16-23)
db 10010010b ; 1st flags, type flags
db 11001111b ; 2nd flags, Limit (bits 16-19)
db 0x0 ;  Base (bits 24-31)

.gdt_end:

.gdt_descriptor:
dw .gdt_end - .gdt_start - 1 ; size of gdt
dd .gdt_start ; start address of gdt

; constants for segment registers
CODE_SEG equ .gdt_code - .gdt_start
DATA_SEG equ .gdt_data - .gdt_start
%endif
