#!/usr/bin/env bash

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd)
KERNEL=$SCRIPT_DIR/../bin/AmberOS.elf

qemu-system-i386 -kernel $KERNEL \
  -no-reboot \
  -d int,cpu_reset \
  $@
