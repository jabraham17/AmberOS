%ifndef _IC_PRINT32_ASM_
%define _IC_PRINT32_ASM_
bits 32

VIDEO_MEMORY equ 0xb8000
ROWS equ 25
COLS equ 80
NCHARS equ ROWS*COLS
WHITE_ON_BLACK equ 0x0f

; clear_screen_32:
;     pusha

;     mov edx, VIDEO_MEMORY
;     mov ecx, NCHARS
;     ; each char is 2 bytes
;     ; we can write 4 bytes at a time
;     ; so we can divide NCHARS by 2
;     shr ecx, 1

;     .loop_top:
;     cmp ecx, 0
;     je .loop_bottom
;     mov dword [edx], 0
;     add edx, 4
;     sub ecx, 1
;     jmp .loop_top
;     .loop_bottom:

;     popa
;     ret

; eax has pointer to null string
print_str_32:
    pusha

    mov ecx, eax ; store pointer in ecx

    ; load last line into ax
    xor eax, eax
    mov al, [globals.current_screen_row]
    ; multiply ax by 80
    xor edx, edx
    mov dx, 80
    mul dx
    mov dx, ax ; store result in si
    shl dx, 1
    add edx, VIDEO_MEMORY ; add to video memory offset

    .loop_top:
    mov al, [ecx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je .loop_bottom

    mov [edx], ax ; store char
    add ecx, 1 ; next char
    add edx, 2 ; next cell in mem
    jmp .loop_top
    .loop_bottom:

    ; get current line
    sub edx, VIDEO_MEMORY ; get rid of offset
    mov ax, dx
    mov dx, COLS
    div dl ; current line in al
    shr al, 1
    add al, 1 ; add 1 for next row
    mov [globals.current_screen_row], al

    popa
    ret

; eax conatins the hex value to be printed
print_hex_32:
%if 0
    pusha

    ; loop 8 times for 8 hex values
    xor ebx, ebx

    ; used for shift
    ; start at 28 and work down so we print proper because little endian
    mov ecx, 28

    .loop_top:
    cmp ebx, 0x08
    je .loop_bottom

    ; load value and shift and mask
    mov edx, eax
    shr edx, cl
    and edx, [.mask]

    ; if lt 0xA, add 0x30
    ; if ge 0xA, add 0x41-0xA
    cmp edx, 0xA
    jge .ge
    .lt:
    add edx, 0x30
    jmp .done
    .ge:
    add edx, 0x41-0xA

    .done:
    ; store into hex string
    mov byte [.hex_string+2+ebx], dl

    
    add ebx, 0x01
    sub ecx, 0x04
    jmp .loop_top
    .loop_bottom:

    ; print created hex string
    mov eax, .hex_string
    call print_str_32

    popa
    ret
    .hex_string: db "0x00000000",0
    .mask: dd 0x0000000F
%else
ret
%endif

%endif
