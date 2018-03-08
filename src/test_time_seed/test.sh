make clean
make -j8 && ./test_time_seed
make clean
make -j8 -f Makefile.2 && ./test_time_seed
