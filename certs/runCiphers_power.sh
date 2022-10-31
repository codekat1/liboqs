rm -rf Signatures_power.o

# -g for debugging
g++ -g -o Signatures_power.o Signatures_power.cpp -L../build/lib -loqs
g++ -o runRSA_power.o runRSA_power.cpp

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

date

./Signatures_power.o 

./runRSA_power.o

date
