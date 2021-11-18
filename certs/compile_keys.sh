rm -rf Keys.o

#g++ -o Keys.o Keys.cpp -L../build/lib -loqs
g++ -o Keys.o Keys.cpp -L../build/lib -loqs

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

./Keys.o