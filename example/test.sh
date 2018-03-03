cd ../src/ && \
. ./module.sh

make -j8 && \
cd ../example && \
../src/md -imd.inp && \
time -p ../src/md -imd2.inp
