rm -rf gdb_baseline.o
rm -rf gdb_keygen.o
rm -rf gdb_sign.o
rm -rf gdb_verify.o

# -g for debugging
g++ -g -o gdb_baseline.o gdb_baseline.cpp -L../build/lib -loqs
g++ -g -o gdb_keygen.o gdb_keygen.cpp -L../build/lib -loqs
g++ -g -o gdb_sign.o gdb_sign.cpp -L../build/lib -loqs
g++ -g -o gdb_verify.o gdb_verify.cpp -L../build/lib -loqs

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

./gdb_baseline.o
./gdb_keygen.o Dilithium2
./gdb_sign.o Dilithium2
./gdb_verify.o Dilithium2


#gdb -nh -batch -ex 'py fileName="gdb_keygen.cpp"' -ex 'py logTerminatesAtLine=30' -ex 'source python_gdb_trace.py' -ex 'break log' -ex 'run' -ex 'trace-asm' --args gdb_keygen.o "Dilithium2"
#gdb -nh -batch -ex 'py algorithm="Dilithium 2"' -ex 'py operation="Keygen"' -ex 'set breakpoint pending on' -ex 'py fileName="run.cpp"' -ex 'py logTerminatesAtLine=30' -ex 'source python_gdb_trace.py' -ex 'break log' -ex 'run' -ex 'trace-asm' --args run.o "Dilithium2"



