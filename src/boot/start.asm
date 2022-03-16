; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot
 

section .multiboot.data
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
 
section .bootstrap_stack
stack_bottom:
align 16
    resb 16384 ; 16 KiB, 4 pages
stack_top:

; define page table
section .bss
align 4096
global boot_page_directory
boot_page_directory:
	resb 4096
global boot_page_table1
boot_page_table1:
	resb 4096
 

section .multiboot.text
global _start:function (_start.end - _start)
_start:
    ; very first thing, save multiboot eax and ebx
    extern dm_mbd
    extern dm_magic
    mov [dm_mbd], ebx
    mov [dm_magic], eax


    ; load stack
	mov esp, stack_top

    ; call init functions
    extern gdt_init
    call gdt_init

    extern idt_init
    call idt_init

    extern pic_init
    call pic_init

    extern isr_init
    call isr_init

    extern pit_init
    push 50
    call pit_init ;50hz clock
    add esp, 4 ; pop nowhere
    extern SC_init
    call SC_init

    ; enable paging


	extern kernel_main
	call kernel_main
 
    ; hang forever
	cli
.hang:	hlt
	jmp .hang
.end:
