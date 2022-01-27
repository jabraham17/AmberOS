#!/bin/sh
qemu-system-i386 -drive format=raw,file=image.iso -d cpu_reset -monitor stdio $@
