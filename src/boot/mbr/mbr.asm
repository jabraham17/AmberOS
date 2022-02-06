%ifndef _IC_MBR_MBR_ASM_
%define _IC_MBR_MBR_ASM_

bits 16
org 0x0600 ; correct all labels to correct sector
jmp _mbr_start_old

    TIMES 3-($-$$) DB 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; Dos 4.0 EBPB 1.44MB floppy
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512
    sectPerCluster:    db    1
    reservedSectors:   dw    1
    numFAT:            db    2
    numRootDirEntries: dw    224
    numSectors:        dw    2880
    mediaType:         db    0xf0
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29
    volumeID:          dd    0x2d7e5a1a
    volumeLabel:       db    "CUSTOM-OS  "
    fileSysType:       db    "FAT12   "



%ifndef STAGE1_NSECTORS
%error "STAGE1_NSECTORS must be defined"
%endif

; https://wiki.osdev.org/MBR_(x86)#x86_Examples
_mbr_start_old: ; it is 'old' because we immediatbly copy it tto 0x7c00
    cli ; no interrupt
    xor ax, ax ; zero ax, segment registers (not CS) and sp
    mov ds, ax
    mov es, ax
    mov ss, ax
    xor sp, sp

    ; copy MBR to 7c00, 256 words (512 bytes)
    mov cx, 0x0100 
    mov si, 0x7c00
    mov di, 0x0600
    rep movsw
    jmp 0:_mbr_start ; we have executed copy of code, jump to it

_mbr_start:
    sti ; allow interrupt
    mov byte [globals.boot_drive], dl

    ; make bad assumption that PT1 is boot partition
    ;mov bx, mbr_t.pt1
    ;mov word [globals.pt_offset], bx

_disk_load:
    ; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive 
    mov ah, 0x02
    ;mov al, byte [bx + pt_entry_t.nsectors] ; load only the lowest byte of nsectors
    mov al, STAGE1_NSECTORS
    mov ch, 0 ; default
    ;mov cl, byte [bx + pt_entry_t.lba_start] ; load only the lowest byte of lba start
    mov cl, 2
    mov dh, 0 ; default
    ; dl already has drive
    ; es:bx is buffer
    ; we want to load to 7c00, where boot expects to be
    push bx
    mov bx, 0x7c00
    int 0x13 

    jc .error ; if error
    pop bx
    ;cmp al, byte [bx + pt_entry_t.nsectors] ; did we read enough?
    cmp al, STAGE1_NSECTORS
    jne .error

    ; success, jump to bootloader
    mov si, word [globals.pt_offset]      ; Set DS:SI to Partition Table Entry
    mov dl, byte [globals.boot_drive]
    jmp 0x7C00

    .error:
        mov ah, 0x0e
        mov al, "E"
        int 0x10
        mov al, "r"
        int 0x10
        mov al, "r"
        int 0x10
        mov al, "o"
        int 0x10
        mov al, "r"
        int 0x10
        jmp $ ; halt



globals:
    .boot_drive: db 0
    .pt_offset: dw 0 ; boot partition

times (0x1B8 - ($-$$)) db 0    ; Pad For MBR Partition Table


; LBA to CHS
; Temp = LBA / (Sectors per Track)
; Sector = (LBA % (Sectors per Track)) + 1
; Head = Temp % (Number of Heads)
; Cylinder = Temp / (Number of Heads)

; struc pt_entry_t
;     .attributes: resb 1
;     .chs_start: resb 3
;     .type: resb 1
;     .chs_end: resb 3
;     .lba_start: resd 1
;     .nsectors: resd 1
; endstruc

; mbr_t:
;     .disk_id: dd 0
;     .reserved: dw 0
;     .pt1: 
;     istruc pt_entry_t
;         at pt_entry_t.attributes, db 0x80 ; mark as bootable
;         at pt_entry_t.chs_start, db 0,0,2
;         at pt_entry_t.type, db 0x01
;         at pt_entry_t.chs_end, db 0,0,(2+STAGE1_NSECTORS)
;         at pt_entry_t.lba_start, dd 2 ; assume its the next LBA
;         at pt_entry_t.nsectors, dd STAGE1_NSECTORS ; from Makefile
;     iend
;     .pt2: times pt_entry_t_size db 0
;     .pt3: times pt_entry_t_size db 0
;     .pt4: times pt_entry_t_size db 0
;     .signature: dw 0xAA55

times (510 - ($-$$)) db 0
dw 0xAA55

%endif
