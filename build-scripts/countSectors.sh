#!/bin/sh
# count number of 512 sectors a binary file will occupy

size=$(wc -c <$1)
# ceil division
#(divide+by-1)/by
nsectors=$(expr $(expr $size + 512 - 1) / 512)
echo $nsectors
