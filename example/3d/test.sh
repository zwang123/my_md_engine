cd ../../src/ && \
. ./module.sh

#make clean && \
make -j8 && \
cd - && \
time -p ../../src/md -imd.inp
