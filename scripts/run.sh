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
DRIVE=$SCRIPT_DIR/../bin/image.iso

# initial setup stuff
# mkfifo $SCRIPT_DIR/monitor.in $SCRIPT_DIR/monitor.out
# mkfifo $SCRIPT_DIR/serial.in $SCRIPT_DIR/serial.out

# start detatched qemu
if [ $DEBUG_MODE ]; then
tmux new-window
EXTRA_ARGS="-s -S"
else
tmux new-session -d -s _qemu
fi

tmux rename-window "_qemu_window"
tmux send "qemu-system-i386 \
-drive format=raw,file=$DRIVE \
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
