#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <oqs/oqs.h>

int main() {
	std::string algorithm = "dilithium2";
	char* algorithm_char = const_cast<char*>(algorithm.c_str());
	std::cout << "Testing testing 123" << std::endl;
	//std::cout << OQS_SIG_alg_picnic_L1_FS << std::endl;
	std::cout << OQS_H << std::endl;
	//std::cout << OQS_SIG_alg_is_enabled(algorithm_char) << std::endl;
}