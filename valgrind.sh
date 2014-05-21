export LD_LIBRARY_PATH=./build/install/testExecutable/release/lib/
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH
valgrind $LD_LIBRARY_PATH/test
