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

SCRIPT_NAME=
if [ -z $BASH_SOURCE ]; then
    SCRIPT_NAME=$0
else
    SCRIPT_NAME=${BASH_SOURCE[0]}
fi

SCRIPT_DIR=$(realpath "$(dirname "$SCRIPT_NAME")")
ELF=$SCRIPT_DIR/../bin/AmberOS.elf

tmux new-session -d -s _qemu "DEBUG_MODE=1 $SCRIPT_DIR/run.sh"

tmux new-window 
tmux rename-window "_gdb_window"

tmux send "gdb $ELF \
-x $SCRIPT_DIR/gdb_cmds \
-ex \"target remote :1234\" \
$@ \
&& tmux kill-session -t _qemu" ENTER
# -ex to do more commands

tmux attach
