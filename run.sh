#!/bin/sh
qemu-system-i386 \
-drive format=raw,file=bin/image.iso \
-no-reboot -d int,cpu_reset \
-monitor stdio \
$@
