rm -rf test.o

g++ -o test.o test.cpp -L../build/lib -loqs
./test.o