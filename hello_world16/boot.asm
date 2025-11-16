bits 16
org 0x7c00 ; correct all labels to correct sector
jmp _start

%include "print.asm"
%include "disk_load.asm"

halt:
    jmp $ ; jump to current address

mystr:
    db "Hello World",10, 0

_start:

    ; save boot drive
    mov [globals.boot_drive], dl

    ; setup stack
    mov bp, 0x8000
    mov sp, bp

    ; load 5 sectors to 0x0000(ES):0x9000(BX)
    mov al, 0x5
    mov bx, 0x9000
    mov cl, [globals.boot_drive]
    call disk_load

    mov ax, mystr
    call print_str
    call go_to_new_line

    mov ax, 0xDEAF
    call print_hex
    mov ax, 0x7190
    call print_hex


    call go_to_new_line
    call go_to_new_line

    ; print first word from first sector loaded
    mov ax, [bx]
    call print_hex
    ; print first word from second sector loaded
    mov ax, [bx + 512] 
    call print_hex


    jmp halt


; global variables
globals:
.boot_drive: db 0


%define pad_size 510-($-$$)
times pad_size db 0
dw 0xaa55
