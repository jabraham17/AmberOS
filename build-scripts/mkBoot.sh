#!/bin/sh
# make a grub boot disk
# $1 is kernel name
# $2 is bin directory

KERNEL_NAME=$1
KERNEL_ELF=$KERNEL_NAME.elf
KERNEL_ISO=$KERNEL_NAME.iso
BIN=$2
GRUB_CFG=$2/grub.cfg

if [ ! -f $KERNEL_ELF ]; then
    echo "Error: Could not find $KERNEL_ELF"
    exit 1
fi
if [ ! -d $BIN ]; then
    echo "Error: Could not find $BIN"
    exit 1
fi
grub2-file --is-x86-multiboot $KERNEL_ELF
if [ $? -eq 1 ]; then
    echo "Error: $KERNEL_ELF is not multiboot"
    exit 1
fi


# make grub cfg
OSNAME=`basename $KERNEL_NAME`
echo "menuentry \"$OSNAME\" {
    multiboot /boot/$OSNAME.elf
}" > $GRUB_CFG

# mount temp filesysrtem and make grub disk
mkdir -p $BIN/isodir/boot/grub
cp $KERNEL_ELF $BIN/isodir/boot/$OSNAME.elf
cp $GRUB_CFG $BIN/isodir/boot/grub/grub.cfg
grub2-mkrescue -o $KERNEL_ISO $BIN/isodir
