rm -rf Signatures.o

g++ -o Signatures.o Signatures.cpp -L../build/lib -loqs

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

./Signatures.o