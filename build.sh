#!/usr/bin/env bash 
echo $1
gcc -I/usr/local/include/libusb-1.0 $1 -L/usr/local/lib -lusb-1.0

./a.out 
