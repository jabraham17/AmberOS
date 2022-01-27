#!/bin/sh
qemu-system-i386 -drive format=raw,file=bin/image.iso -d cpu_reset -monitor stdio $@
