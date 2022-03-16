#!/bin/sh

realpath() {
  OURPWD=$PWD
  cd "$(dirname "$1")"
  LINK=$(readlink "$(basename "$1")")
  while [ "$LINK" ]; do
    cd "$(dirname "$LINK")"
    LINK=$(readlink "$(basename "$1")")
  done
  REALPATH="$PWD/$(basename "$1")"
  cd "$OURPWD"
  echo "$REALPATH"
}
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")

tmux new-session -d -s _qemu "DEBUG_MODE=1 $SCRIPT_DIR/run.sh"

tmux new-window 
tmux rename-window "_gdb_window"

tmux send "gdb bin/AmberOS.elf \
-x $SCRIPT_DIR/gdb_cmds \
-ex \"target remote :1234\" \
$@ \
&& tmux kill-session -t _qemu" ENTER
# -ex to do more commands

tmux attach
