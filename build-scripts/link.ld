OUTPUT_ARCH(i386:x86)
/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* load at 1M */
	. = 0x00100000;

    /* all vars decalred in here are values, NOT ADDRESS
        Must take the address of them to get physical addresses
    */
 
    __multiboot_start = .;
    .multiboot.data : {
       *(.multiboot.data)
    }
    .multiboot.text : {
        *(.multiboot.text)
    }
    __multiboot_end = .;

    __kernel_start = .;
	.text ALIGN(4K) :
	{
        __kernel_start_a = .;
		*(.text)
	}
	.rodata ALIGN(4K) :
	{
		*(.rodata*)
	}
	.data ALIGN(4K) :
	{
		*(.data)
	}
	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
    __kernel_end = .;

}
