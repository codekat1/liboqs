rm -rf Signatures.o

# -g for debugging
g++ -g -o Signatures.o Signatures.cpp -L../build/lib -loqs
g++ -o runRSA runRSA.cpp

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

date

./Signatures.o 

./runRSA

date
