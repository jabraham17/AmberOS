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
DRIVE=$SCRIPT_DIR/../bin/AmberOS.iso

# initial setup stuff
test -e $SCRIPT_DIR/monitor.in && rm -f $SCRIPT_DIR/monitor.in
test -e $SCRIPT_DIR/monitor.out && rm -f $SCRIPT_DIR/monitor.out
test -e $SCRIPT_DIR/serial.in && rm -f $SCRIPT_DIR/serial.in
test -e $SCRIPT_DIR/serial.out && rm -f $SCRIPT_DIR/serial.out
mkfifo $SCRIPT_DIR/monitor.in $SCRIPT_DIR/monitor.out
mkfifo $SCRIPT_DIR/serial.in $SCRIPT_DIR/serial.out
test -f $SCRIPT_DIR/runlog.txt && echo "" > $SCRIPT_DIR/runlog.txt

# start detatched qemu
if [ $DEBUG_MODE ]; then
tmux new-window
EXTRA_ARGS="-s -S"
else
tmux new-session -d -s _qemu
fi

tmux rename-window "_qemu_window"
tmux send "qemu-system-i386 \
-cdrom $DRIVE \
-no-reboot \
-d int,cpu_reset \
-D $SCRIPT_DIR/runlog.txt \
-monitor pipe:$SCRIPT_DIR/monitor \
-serial pipe:$SCRIPT_DIR/serial \
$EXTRA_ARGS \
$@ \
&& tmux kill-session -t _qemu" ENTER


# open window for debug/log stuff
if [ $DEBUG_MODE ] && [ -n "$TMUX" ]; then
tmux select-window -t "_gdb_window"
GDB_PANE=$TMUX_PANE
tmux split-window
else
tmux new-window
tmux rename-window "_log_window"
fi

tmux send "cat $SCRIPT_DIR/monitor.out" ENTER
# open pane for serial output
tmux split-window -h
tmux send "cat $SCRIPT_DIR/serial.out" ENTER
# open pane for log
tmux split-window -h
tmux send "tail -f $SCRIPT_DIR/runlog.txt" ENTER

# even distrubute and make visible
if [ $DEBUG_MODE ]; then
tmux select-layout main-horizontal
else
tmux select-layout even-horizontal
fi

if [ $DEBUG_MODE ]; then
tmux select-pane -U
fi

tmux attach

# idle waiting for abort from user
# read -r -d '' _ </dev/tty

# rm -f $SCRIPT_DIR/monitor.in $SCRIPT_DIR/monitor.out
# rm -f $SCRIPT_DIR/serial.in $SCRIPT_DIR/serial.out
