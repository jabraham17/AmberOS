%ifndef _IC_HELPER_PRINT16_ASM_
%define _IC_HELPER_PRINT16_ASM_
bits 16

; because of segement register, if we put this in es we it is extended to full 0xb8000
VIDEO_MEMORY_16 equ 0xb800
ROWS equ 25
COLS equ 80
NCHARS equ ROWS*COLS
WHITE_ON_BLACK equ 0x0f
; 0xb8000 + 2 * (row * 80 + col)

clear_screen_16:
    pusha
    push es

    mov dx, VIDEO_MEMORY_16
    mov es, dx

    xor bx, bx
    ; each char is 2 bytes
    ; we can write 2 bytes at a time

    .loop_top:
    cmp bx, NCHARS
    je .loop_bottom
    mov dword [es:bx], 0
    add bx, 2
    jmp .loop_top
    .loop_bottom:

    pop es
    popa
    ret

; ax has pointer to null string
print_str_16:
    pusha
    push es

    mov dx, VIDEO_MEMORY_16
    mov es, dx


    mov cx, ax ; store pointer in ecx

    ; load last line into ax
    xor ax, ax
    mov al, [globals.current_screen_row]
    ; multiply ax by 80
    mov dx, 80
    mul dl
    mov dx, ax ; store result in dx
    shl dx, 1


    .loop_top:
    mov bx, cx
    mov al, [bx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je .loop_bottom

    mov bx, dx
    mov [es:bx], ax ; store char
    add cx, 1 ; next char
    add dx, 2 ; next cell in mem
    jmp .loop_top
    .loop_bottom:

    ; get current line
    mov ax, dx
    mov dx, COLS
    div dl ; current line in al
    shr al, 1
    add al, 1 ; add 1 for next row
    mov [globals.current_screen_row], al

    pop es
    popa
    ret

; ax conatins the hex value to be printed
print_hex_16:
    pusha

    ; loop 4 times for 4 hex values
    xor bx, bx

    ; used for shift
    ; start at 12 and work down so we print proper because little endian
    mov cx, 12

    .loop_top:
    cmp bx, 0x04
    je .loop_bottom

    ; load value and shift and mask
    mov dx, ax
    shr dx, cl
    and dx, [.mask]

    ; if lt 0xA, add 0x30
    ; if ge 0xA, add 0x41-0xA
    cmp dx, 0xA
    jge .ge
    .lt:
    add dx, 0x30
    jmp .done
    .ge:
    add dx, 0x41-0xA

    .done:
    ; store into hex string
    mov byte [.hex_string+2+bx], dl

    
    add bx, 0x01
    sub cx, 0x04
    jmp .loop_top
    .loop_bottom:

    ; print created hex string
    mov ax, .hex_string
    call print_str_16

    popa
    ret
    .hex_string: db "0x0000",0
    .mask: dw 0x000F

%endif
