#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <oqs/oqs.h>
#include <string>
#include <thread>
#include <vector>



/*
	algorithm: What algorithm the instance of the class is using
	public_key_length: Bytes of the public key
	private_key_length: Bytes of the private key
	signature_length: Bytes of the signature length

	generate_keypair(): Generate the public and private keys
	get_public_key(): Get the public key in a hex string
	get_private_key(): Get the private key in a hex string
	public_key: The public key as a char array
	private_key: The private key as a char array

	sign(message): Return the signature (unsigned char*) for a message (std::string)

	verify(message, signature): Return true or false for if a message and signature are valid
*/
class SignatureManager {
private:
	char* algorithm_char;
	OQS_SIG *sig;

public:
	std::string algorithm;
	unsigned int public_key_length, private_key_length, signature_length;
	unsigned char *public_key, *private_key;

	SignatureManager(std::string _algorithm) {
		algorithm = _algorithm;
		algorithm_char = const_cast<char*>(algorithm.c_str());
		
		if(OQS_SIG_alg_is_enabled(algorithm_char) == 0) {
			throw std::runtime_error("ERROR: Algorithm \"" + algorithm + "\" does not exist");
		}

		OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
		sig = OQS_SIG_new(algorithm_char);

		public_key_length = sig->length_public_key;
		private_key_length = sig->length_secret_key;
		signature_length = sig->length_signature;

		public_key = (unsigned char*) malloc(public_key_length);
		private_key = (unsigned char*) malloc(private_key_length);
	}

	// Generate a public and private key pair
	void generate_keypair() {
		OQS_STATUS status = OQS_SIG_keypair(sig, public_key, private_key);
		if(status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_keypair failed\n");
	}

	// Get the generated public key
	std::string get_public_key() {
		return bytes_to_hex(public_key, public_key_length);
	}

	// Get the generated private key
	std::string get_private_key() {
		return bytes_to_hex(private_key, private_key_length);
	}

	// Sign a message, returns its signature
	unsigned char* sign(std::string message) {
		unsigned char *signature = (unsigned char*) malloc(signature_length);
		unsigned int message_length = message.length();
		size_t *signature_len = (size_t*) &signature_length;
		uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_SIG_sign(sig, signature, signature_len, message_bytes, message_length, private_key);

		if (status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_sign failed\n");

		return signature;
	}

	// Verify a signature
	bool verify(std::string message, unsigned char* signature) {
		unsigned int message_length = message.length();
		//size_t *signature_len = (size_t*) &signature_length;
		uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_SIG_verify(sig, message_bytes, message_length, signature, signature_length, public_key);

		return status == OQS_SUCCESS;
	}

	// Given an array of bytes, convert it to a hexadecimal string
	static std::string bytes_to_hex(unsigned char* bytes, int len) {
		constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		std::string output(len * 2, ' ');
		for (int i = 0; i < len; ++i) {
			output[2 * i] = hexmap[(bytes[i] & 0xF0) >> 4];
			output[2 * i + 1] = hexmap[bytes[i] & 0x0F];
		}
		return output;
	}
};



// Return the header row for the CSV file
std::string benchmarkLogHeader(std::vector<int> message_lengths) {
	std::string row = "";
	row += "Name,";
	for(int i: message_lengths) {
		row += "Allocate L" + std::to_string(i) + ",";
		row += "Keygen L" + std::to_string(i) + ",";
		row += "Sign L" + std::to_string(i) + ",";
		row += "Verify L" + std::to_string(i) + ",";
	}
	return row;
}

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string output = "";
    for (int i = 0; i < len; ++i) {
        output += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return output;
}

// Given an algorithm name, run a benchmark n times and return its CSV row
std::string benchmarkLog(std::string algorithm, std::vector<int> message_lengths, int n, int numberOfAlgorithms) {

	std::string row = "\"" + algorithm + "\",";

	for(int length: message_lengths) {

		std::string message = gen_random(length);

		double clocks_allocate = 0;
		double clocks_keygen = 0;
		double clocks_signing = 0;
		double clocks_verifying = 0;
		std::clock_t t0, t1, t2, t3, t4;
		for(int i = 0; i < n; i++) {
			

			t0 = std::clock();
				SignatureManager sigmanager(algorithm);
			t1 = std::clock();
				sigmanager.generate_keypair();
			t2 = std::clock();
				unsigned char* signature = sigmanager.sign(message);
			t3 = std::clock();
				bool result = sigmanager.verify(message, signature);
			t4 = std::clock();

			clocks_allocate += (t1 - t0);
			clocks_keygen += (t2 - t1);
			clocks_signing += (t3 - t2);
			clocks_verifying += (t4 - t3);
		}

		clocks_allocate /= (double)n;
		clocks_keygen /= (double)n;
		clocks_signing /= (double)n;
		clocks_verifying /= (double)n;

		double ms_allocate = clocks_allocate / (double)(CLOCKS_PER_SEC / 1000);
		double ms_keygen = clocks_keygen / (double)(CLOCKS_PER_SEC / 1000);
		double ms_signing = clocks_signing / (double)(CLOCKS_PER_SEC / 1000);
		double ms_verifying =  clocks_verifying / (double)(CLOCKS_PER_SEC / 1000);

		// Used to quickly grab the key and signature lengths
		//SignatureManager sigmanager(algorithm);

		row += std::to_string(ms_allocate) + ",";
		row += std::to_string(ms_keygen) + ",";
		row += std::to_string(ms_signing) + ",";
		row += std::to_string(ms_verifying) + ",";
	}
	std::cout << "    " << algorithm << " completed" << std::endl;

	return row;
}

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL));

	std::vector<int> message_lengths{0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000}; 

	int numSamples = 100;
	std::cout << "How many samples would you like ";
	std::cin >> numSamples;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	// A list of all available algorithms
	const char *availAlgs[] = {
		// Round 3
		//"picnic_L1_FS", "picnic_L1_UR", "picnic_L1_full", "picnic_L3_FS", "picnic_L3_UR", "picnic_L3_full", "picnic_L5_FS", "picnic_L5_UR", "picnic_L5_full", "picnic3_L1", "picnic3_L3", "picnic3_L5", "qTesla-p-I", "qTesla-p-III", "DILITHIUM_2", "DILITHIUM_3", "DILITHIUM_4", "Falcon-512", "Falcon-1024", "MQDSS-31-48", "MQDSS-31-64", "Rainbow-Ia-Classic", "Rainbow-Ia-Cyclic", "Rainbow-Ia-Cyclic-Compressed", "Rainbow-IIIc-Classic", "Rainbow-IIIc-Cyclic", "Rainbow-IIIc-Cyclic-Compressed", "Rainbow-Vc-Classic", "Rainbow-Vc-Cyclic", "Rainbow-Vc-Cyclic-Compressed", "SPHINCS+-Haraka-128f-robust", "SPHINCS+-Haraka-128f-simple", "SPHINCS+-Haraka-128s-robust", "SPHINCS+-Haraka-128s-simple", "SPHINCS+-Haraka-192f-robust", "SPHINCS+-Haraka-192f-simple", "SPHINCS+-Haraka-192s-robust", "SPHINCS+-Haraka-192s-simple", "SPHINCS+-Haraka-256f-robust", "SPHINCS+-Haraka-256f-simple", "SPHINCS+-Haraka-256s-robust", "SPHINCS+-Haraka-256s-simple", "SPHINCS+-SHA256-128f-robust", "SPHINCS+-SHA256-128f-simple", "SPHINCS+-SHA256-128s-robust", "SPHINCS+-SHA256-128s-simple", "SPHINCS+-SHA256-192f-robust", "SPHINCS+-SHA256-192f-simple", "SPHINCS+-SHA256-192s-robust", "SPHINCS+-SHA256-192s-simple", "SPHINCS+-SHA256-256f-robust", "SPHINCS+-SHA256-256f-simple", "SPHINCS+-SHA256-256s-robust", "SPHINCS+-SHA256-256s-simple", "SPHINCS+-SHAKE256-128f-robust", "SPHINCS+-SHAKE256-128f-simple", "SPHINCS+-SHAKE256-128s-robust", "SPHINCS+-SHAKE256-128s-simple", "SPHINCS+-SHAKE256-192f-robust", "SPHINCS+-SHAKE256-192f-simple", "SPHINCS+-SHAKE256-192s-robust", "SPHINCS+-SHAKE256-192s-simple", "SPHINCS+-SHAKE256-256f-robust", "SPHINCS+-SHAKE256-256f-simple", "SPHINCS+-SHAKE256-256s-robust", "SPHINCS+-SHAKE256-256s-simple"
		
		// Dilithium and Falcon
		"DILITHIUM_2", "DILITHIUM_3", "DILITHIUM_4", "Falcon-512", "Falcon-1024",

		// Rainbow
		"Rainbow-Ia-Cyclic", "Rainbow-Ia-Classic", "Rainbow-Vc-Cyclic"
	};
	const int numberOfAlgorithms = sizeof(availAlgs) / sizeof(availAlgs[0]);

	// INFINITE MAIN LOOP
	while(true) {
		std::string fileName = "SAMPLE_" + std::to_string(numSamples) + "_" + gen_random(10) + ".csv";
		std::cout << "Beginning file: " << fileName << std::endl;
		std::ofstream outputFile;
		outputFile.open("INCOMPLETE_" + fileName);
		outputFile << benchmarkLogHeader(message_lengths) << std::endl;

	    for (int i = 0; i < numberOfAlgorithms; i++) {
			//std::cout << "\nProgress: " << (100 * i / float(numberOfAlgorithms)) << "%\n" << std::endl;
			std::string algorithm = availAlgs[i];
	    	try {
				std::string row = benchmarkLog(algorithm, message_lengths, numSamples, numberOfAlgorithms);
				outputFile << row << std::endl;
			} catch(...) {
				std::cout << "!!!!!!!!!!!!!!!!!!!! ERROR " << algorithm << " does not work." << std::endl;
			}
	    }
	    outputFile.close();
	    std::rename(("INCOMPLETE_" + fileName).c_str(), fileName.c_str());
	    std::cout << std::endl << "All data has been successfully saved to " << fileName << "!" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

    return 0;

}


/* The available algorithms are as follows:

"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/