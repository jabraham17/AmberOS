# view disam
/usr/local/opt/llvm/bin/llvm-objcopy -I binary -O elf32-i386 --rename-section=.data=.text,code bin/image.iso _temp.o && /usr/local/opt/llvm/bin/llvm-objdump -d -Mintel _temp.o && rm _temp.o
/usr/local/opt/llvm/bin/llvm-objcopy -I binary -O elf32-i386 --rename-section=.data=.text,code bin/kernel.bin _temp.o && /usr/local/opt/llvm/bin/llvm-objdump -d -Mintel _temp.o && rm _temp.o


booting to hardware
take flash drive, partition schemem Master Boot record, FAT
- this doesnt matter much, as we are going to overwrite it

on mac, this makes the partition table /dev/disk2 and the actual contents at /dev/disk2s1

we will overwrite partition table at /dev/disk2 with `sudo dd if=bin/image.iso of=/dev/disk2`

for some reason, if there is more than ~400~ bytes of stuff in boot.bin, fails to boot and goes straight to other harddrive
- https://stackoverflow.com/questions/47277702/custom-bootloader-booted-via-usb-drive-produces-incorrect-output-on-some-compute/47320115#47320115
- ~~hypotheisi: BIOS is overwriting values with BPB~~
- hypothesis: need to setup MBR or GPT, likely GPT but MBR will be much easier
  - https://wiki.osdev.org/Partition_Table
  - https://wiki.osdev.org/GPT
  - https://wiki.osdev.org/MBR_(x86)
