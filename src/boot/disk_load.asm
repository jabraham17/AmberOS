%ifndef _IC_DISK_LOAD_ASM_
%define _IC_DISK_LOAD_ASM_
bits 16

; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive 
; load al sectors from disk
; bx is base addr for where to load to
; from disk dl
disk_load:
    pusha
    push ax ; explicity save num sectors extra, we will pop in a moment

    mov ah, 0x02
    ; num sectors already in al
    ; disk number already in dl
    ; base address already in bx
    mov dh, 0x00 ; head at 0
    mov cx, 0x0002 ; read cyl 0x00 and sector 0x02, boot sector already loaded

    int 0x13 
    ; our result is in es:bx
    ; al has number sectors read
    ; error if CF
    ; error is al != num sectors requested
    jc .error

    pop dx
    cmp dl, al
    jne .error

    popa
    ret

    .error:
        mov ax, .disk_error_msg
        call print_str_16
        jmp $ ; halt

    .disk_error_msg:
    db "Disk Error", 0
%endif
