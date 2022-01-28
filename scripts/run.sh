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

qemu-system-i386 \
-drive format=raw,file=$DRIVE \
-no-reboot -d int,cpu_reset \
-monitor stdio \
$@
