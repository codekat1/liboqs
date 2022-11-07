#include <stdio.h>
#include <iostream>

using namespace std;




















void log() {
	int i = 0, a = 5;
	for(i = 0; i < 100; i++) {
		a = i * a + 1;
	}
	return;
}

int main(int argc, char** argv) {
	for(int i = 0; i < 10; i++) {
		log();
	}


	//cout << a << endl;

}

//gdb run.o