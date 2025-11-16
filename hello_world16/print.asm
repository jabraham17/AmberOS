
; https://en.wikipedia.org/wiki/INT_10H

; ax contains a pointer to our null terminated string
print_str:
    pusha
    mov bx, ax ; save pointer to bx

    mov ah, 0x0e ; bios teletype

    .loop_top:
    cmp byte [bx], 0x00
    je .loop_bottom
    mov al, [bx]
    int 0x10
    add bx, 0x01
    jmp .loop_top
    .loop_bottom:

    popa
    ret

go_to_new_line:
    pusha

    ; zero bx
    xor bx, bx

    ; get cursor postition, dx has row/col
    mov ax, 0x0300
    int 0x10

    ; add one to row
    add dx, 0x0100
    ; zero col
    and dx, 0xFF00

    ; set cursor postition, dx has row/col
    mov ax, 0x0200
    int 0x10

    popa
    ret


; ax conatins the hex value to be printed
print_hex:
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
    call print_str

    popa
    ret
    .hex_string: db "0x0000",0
    .mask: dw 0x000F



