#!/bin/sh
# count number of 512 sectors a binary file will occupy

if [ ! -f $1 ]; then
    echo "0"
else
    size=$(wc -c <$1)
    # ceil division
    #(divide+by-1)/by
    nsectors=$(expr $(expr $size + 512 - 1) / 512)
    echo $nsectors
fi
