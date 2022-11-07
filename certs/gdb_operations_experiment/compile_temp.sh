rm -rf gdb_temp.o
g++ -g -o gdb_temp.o gdb_temp.cpp -L../build/lib -loqs

./gdb_temp.o

#gdb -nh -batch -ex 'source python.py' -ex 'break main' -ex 'run' -ex 'trace-asm' ./gdb_temp.o
#gdb -nh -batch -ex 'py algorithm="Dilithium 2"' -ex 'py operation="Keygen"' -ex 'set breakpoint pending on' -ex 'py fileName="gdb_temp.cpp"' -ex 'py logTerminatesAtLine=30' -ex 'source python_gdb_trace.py' -ex 'break log' -ex 'run' -ex 'trace-asm' --args gdb_temp.o "Dilithium2"
