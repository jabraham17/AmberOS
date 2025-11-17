#include "detectmem.h"
#include <drivers/screen/screen.h>
#include <stdlib/error.h>

// multiboot vars
multiboot_info_t* dm_mbd;
uint32_t dm_magic;

void dm_printmm() {

  /* Make sure the magic number matches for memory mapping*/
  if(dm_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("invalid magic number!");
  }

  /* Check bit 6 to see if we have a valid memory map */
  if(!(dm_mbd->flags >> 6 & 0x1)) {
    panic("invalid memory map given by GRUB bootloader");
  }

  /* Loop through the memory map and display the values */
  for(unsigned int i = 0; i < dm_mbd->mmap_length;
      i += sizeof(multiboot_memory_map_t)) {
    multiboot_memory_map_t* mmmt =
        (multiboot_memory_map_t*)(dm_mbd->mmap_addr + i);

    SC_printf(
        "Start Addr: 0x%x%x | Length: 0x%x%x | Size: 0x%x | Type: %d: ",
        mmmt->addr_high, mmmt->addr_low, mmmt->len_high, mmmt->len_low,
        mmmt->size, mmmt->type);

    if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
      SC_printf("free");
      /*
       * Do something with this memory block!
       * BE WARNED that some of memory shown as availiable is actually
       * actively being used by the kernel! You'll need to take that
       * into account before writing to memory!
       */
    }
    SC_printf("\n");
  }
}
