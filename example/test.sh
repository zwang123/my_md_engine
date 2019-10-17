#!/bin/bash

cd ../src/ && \
. ./module.sh

make clean && \
make -j8 -f Makefile.1 && \
cd ../example && \
../src/md -imd.inp && \
time -p ../src/md -imd2.inp
