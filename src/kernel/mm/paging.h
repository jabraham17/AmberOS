#ifndef _KERNEL_MM_PAGING_H_
#define _KERNEL_MM_PAGING_H_

#include <stdlib/types.h>

typedef struct page {
    u32int present : 1;  // Page present in memory
    u32int rw : 1;       // Read-only if clear, readwrite if set
    u32int user : 1;     // Supervisor level only if clear
    u32int accessed : 1; // Has the page been accessed since last refresh?
    u32int dirty : 1;    // Has the page been written to since last refresh?
    u32int unused : 7;   // Amalgamation of unused and reserved bits
    u32int frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    // Array of pointers to pagetables.
    page_table_t* tables[1024];
} page_directory_t;

extern page_directory_t* boot_page_directory;
extern page_table_t* boot_page_table1;

#endif
