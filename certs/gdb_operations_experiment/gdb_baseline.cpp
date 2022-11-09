#include <stdio.h>
#include <iostream>

using namespace std;

// Log's return must be on line 30 for GDB to pick up on the function ending






















void log() {
	return; // Line 30
}

int main(int argc, char** argv) {
	log();
}