#!/bin/bash

# export OPENCL_PATH=/home/s1350924/opencl/fbdev
# export OPENC;_PATH=/usr/local/lib/mgd/
export OPENCL_PATH=/usr/local/lib/mali/fbdev
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OPENCL_PATH

gcc -o gauss_jordan gauss_jordan.c -I/usr/include -L$OPENCL_PATH -Wl,-rpath,$OPENCL_PATH -lOpenCL && ./gauss_jordan
