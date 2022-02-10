#!/bin/sh
# given file in $1, pad it to nearest 512
# if given $2, to it to $2*512

if [ -f $1 ]; then    
    size=$(wc -c <$1)
    # ceil division
    #(divide+by-1)/by
    if [ -z $2 ]; then
        nsectors=$(expr $(expr $size + 512 - 1) / 512)
    else
        nsectors=$2
    fi

    targetsize=$(expr 512 \* $nsectors)

    if [ $size -ne $targetsize ]; then
        # seek to last byte of file and write a zero, this will expand whole file
        dd if=/dev/zero of=$1 bs=1 seek=$(expr $targetsize - 1) count=1 2>/dev/null
    fi
fi
