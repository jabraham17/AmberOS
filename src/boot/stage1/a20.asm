%ifndef _IC_STAGE1_A20_ASM_
%define _IC_STAGE1_A20_ASM_

bits 16

; Test if A20 is already enabled - if it is you don't need to do anything at all
; Try the BIOS function. Ignore the returned status.
; Test if A20 is enabled (to see if the BIOS function actually worked or not)
; Try the keyboard controller method.
; Test if A20 is enabled in a loop with a time-out (as the keyboard controller method may work slowly)
; Try the Fast A20 method last
; Test if A20 is enabled in a loop with a time-out (as the fast A20 method may work slowly)
; If none of the above worked, give up

enable_a20:
    pusha
    call check_a20
    cmp ax, 1
    mov ax, .a20_already_enable
    je .done ; already enabled, done     

    call enable_a20_bios
    cmp ax, 1
    mov ax, .a20_bios_enable
    je .done ; enabled, done

    call enable_a20_keyboard
    cmp ax, 1
    mov ax, .a20_keyboard_enable
    je .done ; enabled, done

    call enable_a20_fast
    cmp ax, 1
    mov ax, .a20_fast_enable
    je .done ; enabled, done

    ; if we get here, complelty failed to enable a20, print error and hang
    mov ax, .a20_enable_error
    call print_str_16
    jmp $ ; halt

.done:
    call print_str_16
    popa
    ret

.a20_enable_error:
    db "Failed to enable A20 line", 0
.a20_already_enable:
    db "A20 Line already enabled", 0
.a20_bios_enable:
    db "A20 Line enabled with BIOS", 0
.a20_keyboard_enable:
    db "A20 Line enabled with keyboard", 0
.a20_fast_enable:
    db "A20 Line enabled with FAST method", 0


; returns 0 if disabled, 1 if enabled
check_a20:
    pushf
    push ds
    push es
    push di
    push si
    cli
 
    xor ax, ax
    mov es, ax
    not ax ; ax = 0xFFFF
    mov ds, ax


    mov di, 0x0500
    mov si, 0x0510

    mov al, byte [es:di]
    push ax
    mov al, byte [ds:si]
    push ax

    ; if A20 is not enabled, we should be writing values at the same address
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    cmp byte [es:di], 0xFF ; compare two bytes

    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al

    mov ax, 0
    je .done ; uses cmp from above
 
    mov ax, 1
 
    .done:
    sti
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret


; I am inrediblty lazy, both QEMU and my physical testing machine have A20 already enabled
; So I can skip all of this 


enable_a20_bios:
    mov ax, 0
    ret

enable_a20_keyboard:
    mov ax, 0
    ret

enable_a20_fast:
    mov ax, 0
    ret

%endif
