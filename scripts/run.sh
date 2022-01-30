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
if [ "$TERM" = "screen" ] && [ -n "$TMUX" ]; then
tmux new-window
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
$@ \
&& tmux kill-session -t _qemu" ENTER


# open window for debug/log stuff
if [ "$TERM" = "screen" ] && [ -n "$TMUX" ]; then
tmux select-window -t "_gdb_window"
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
#tmux select-layout even-horizontal
tmux select-layout main-horizontal
#tmux select-layout tiled
tmux attach

# idle waiting for abort from user
# read -r -d '' _ </dev/tty

# rm -f $SCRIPT_DIR/monitor.in $SCRIPT_DIR/monitor.out
# rm -f $SCRIPT_DIR/serial.in $SCRIPT_DIR/serial.out
