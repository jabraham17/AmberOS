#!/bin/sh
./run.sh -s -S &
gdb bin/kernel.elf \
-x gdb_cmds \
-ex "target remote :1234" \
$@
# -ex to do more commands
