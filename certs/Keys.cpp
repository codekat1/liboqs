#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <oqs/oqs.h>

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

	encap(message): Return the signature (unsigned char*) for a message (std::string)

	decap(message, signature): Return true or false for if a message and signature are valid
*/
class KeyManager {
private:
	char* algorithm_char;
	OQS_KEM *kem;

public:
	std::string algorithm;
	unsigned int public_key_length, private_key_length, ciphertext_length, shared_secret_length;
	unsigned char *public_key, *private_key, *ciphertext, *shared_secret_e, *shared_secret_d;

	KeyManager(std::string _algorithm) {
		algorithm = _algorithm;
		algorithm_char = const_cast<char*>(algorithm.c_str());
		
		if(OQS_KEM_alg_is_enabled(algorithm_char) == 0) {
			throw std::runtime_error("ERROR: Algorithm \"" + algorithm + "\" does not exist");
		}

		OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
		kem = OQS_KEM_new(algorithm_char);

		public_key_length = kem->length_public_key;
		private_key_length = kem->length_secret_key;
		private_key_length = kem->length_secret_key;
		ciphertext_length = kem->length_ciphertext;
		shared_secret_length = kem->length_shared_secret;

		public_key = (unsigned char*) malloc(public_key_length);
		private_key = (unsigned char*) malloc(private_key_length);
		ciphertext = (unsigned char*) malloc(ciphertext_length);
		shared_secret_e = (unsigned char*) malloc(shared_secret_length);
		shared_secret_d = (unsigned char*) malloc(shared_secret_length);
	}

	// Generate a public and private key pair
	void generate_keypair() {
		OQS_STATUS status = OQS_KEM_keypair(kem, public_key, private_key);
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

	// Get the ciphertext
	std::string get_ciphertext() {
		return bytes_to_hex(ciphertext, ciphertext_length);
	}

	// Get the secret secret e
	std::string get_shared_secret_e() {
		return bytes_to_hex(shared_secret_e, shared_secret_length);
	}

	// Get the secret secret d
	std::string get_shared_secret_d() {
		return bytes_to_hex(shared_secret_d, shared_secret_length);
	}

	// Encaps the data
	void encaps() {
		//unsigned char *signature = (unsigned char*) malloc(signature_length);
		//unsigned int message_length = message.length();
		//size_t *signature_len = (size_t*) &signature_length;
		//uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);

		if (status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_sign failed\n");

		return;
	}

	// Decaps the data
	void decaps() {
		//unsigned char *signature = (unsigned char*) malloc(signature_length);
		//unsigned int message_length = message.length();
		//size_t *signature_len = (size_t*) &signature_length;
		//uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_KEM_decaps(kem, shared_secret_d, ciphertext, private_key);

		if (status != OQS_SUCCESS) throw std::runtime_error("ERROR: OQS_SIG_sign failed\n");

		return;
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
std::string benchmarkLogHeader() {
	std::string row = "";
	row += "Name,";
	row += "Public Key Length (bytes),";
	row += "Private Key Length (bytes),";
	row += "Ciphertext Length (bytes),";
	row += "Shared Secret Length (bytes),";

	row += "Number of samples,";
	row += "Initialization milliseconds,";
	row += "Key generation milliseconds,";
	row += "Encaps milliseconds,";
	row += "Decaps milliseconds,";
	return row;
}

// Given an algorithm name, run a benchmark n times and return its CSV row
std::string benchmarkLog(std::string algorithm, int n) {

	double clocks_initialization = 0;
	double clocks_keypair_generation = 0;
	double clocks_encaps = 0;
	double clocks_decaps = 0;
	for(int i = 0; i < n; i++) {
		std::clock_t t1 = std::clock();
    		KeyManager keymanager(algorithm);
		std::clock_t t2 = std::clock();
			keymanager.generate_keypair();
		std::clock_t t3 = std::clock();
			keymanager.encaps();
		std::clock_t t4 = std::clock();
			keymanager.decaps();
		std::clock_t t5 = std::clock();

		clocks_initialization += (t2 - t1);
		clocks_keypair_generation += (t3 - t2);
		clocks_encaps += (t4 - t3);
		clocks_decaps += (t5 - t4);
	}

	clocks_initialization /= (double)n;
	clocks_keypair_generation /= (double)n;
	clocks_encaps /= (double)n;
	clocks_decaps /= (double)n;

	double ms_initialization = clocks_initialization / (double)(CLOCKS_PER_SEC / 1000);
	double ms_keypair_generation = clocks_keypair_generation / (double)(CLOCKS_PER_SEC / 1000);
	double ms_encaps = clocks_encaps / (double)(CLOCKS_PER_SEC / 1000);
	double ms_decaps =  clocks_decaps / (double)(CLOCKS_PER_SEC / 1000);

	// Used to quickly grab the key and signature lengths
    KeyManager keymanager(algorithm);

 	std::cout << "    Seconds: i-" << ms_initialization << "/k-" << clocks_keypair_generation << "/s-" << clocks_encaps << "/v-" << clocks_decaps << " ms\t";
	std::cout << algorithm;
	std::cout << std::endl;

	/*
	row += "Name,";
	row += "Public Key Length (bytes),";
	row += "Private Key Length (bytes),";
	row += "Ciphertext Length (bytes),";
	row += "Shared Secret Length (bytes),";

	row += "Number of samples,";
	row += "Initialization milliseconds,";
	row += "Key generation milliseconds,";
	row += "Encaps milliseconds,";
	row += "Decaps milliseconds,";
	*/

	std::string row = "";
	row += "\"" + algorithm + "\",";
	row += std::to_string(keymanager.public_key_length) + ",";
	row += std::to_string(keymanager.private_key_length) + ",";
	row += std::to_string(keymanager.ciphertext_length) + ",";
	row += std::to_string(keymanager.shared_secret_length) + ",";
	row += std::to_string(n) + ",";
	row += std::to_string(ms_initialization) + ",";
	row += std::to_string(ms_keypair_generation) + ",";
	row += std::to_string(ms_encaps) + ",";
	row += std::to_string(ms_decaps) + ",";
	return row;
}

int main(int argc, char** argv) {
	int numSamples = 0;
	std::cout << "How many samples would you like ";
	std::cin >> numSamples;

	std::string fileName = "KEM_algorithm_benchmark_" + std::to_string(numSamples) + ".csv";

	std::ofstream outputFile;
	outputFile.open(fileName);

	outputFile << benchmarkLogHeader() << std::endl;
	
	std::cout << "Listing available algorithms:"<<"\n";

	// A list of all available algorithms
	const char *availAlgs[] = {
		"BIKE1-L1-CPA", "BIKE1-L3-CPA", "BIKE1-L1-FO", "BIKE1-L3-FO", "Classic-McEliece-348864", "Classic-McEliece-348864f", "Classic-McEliece-460896", "Classic-McEliece-460896f", "Classic-McEliece-6688128", "Classic-McEliece-6688128f", "Classic-McEliece-6960119", "Classic-McEliece-6960119f", "Classic-McEliece-8192128", "Classic-McEliece-8192128f", "Kyber512", "Kyber768", "Kyber1024", "Kyber512-90s", "Kyber768-90s", "Kyber1024-90s", "NewHope-512-CCA", "NewHope-1024-CCA", "NTRU-HPS-2048-509", "NTRU-HPS-2048-677", "NTRU-HPS-4096-821", "NTRU-HRSS-701", "LightSaber-KEM", "Saber-KEM", "FireSaber-KEM", "BabyBear", "BabyBearEphem", "MamaBear", "MamaBearEphem", "PapaBear", "PapaBearEphem", "FrodoKEM-640-AES", "FrodoKEM-640-SHAKE", "FrodoKEM-976-AES", "FrodoKEM-976-SHAKE", "FrodoKEM-1344-AES", "FrodoKEM-1344-SHAKE", "SIDH-p434", "SIDH-p434-compressed", "SIDH-p503", "SIDH-p503-compressed", "SIDH-p610", "SIDH-p610-compressed", "SIDH-p751", "SIDH-p751-compressed", "SIKE-p434", "SIKE-p434-compressed", "SIKE-p503", "SIKE-p503-compressed", "SIKE-p610", "SIKE-p610-compressed", "SIKE-p751", "SIKE-p751-compressed"
	};
	const int numberOfAlgorithms = sizeof(availAlgs) / sizeof(availAlgs[0]);
    
    for (int i = 0; i < numberOfAlgorithms; i++) {
		std::string algorithm = availAlgs[i];
    	try {
			std::cout << "Progress: " << (100 * (i + 1) / float(numberOfAlgorithms)) << "%" << std::endl;
			std::string row = benchmarkLog(algorithm, numSamples);
			outputFile << row << std::endl;
		} catch(...) {
			std::cout << "!!!!!!!!!!!!!!!!!!!! ERROR " << algorithm << " does not work." << std::endl;
		}
    }

    outputFile.close();
    std::cout << std::endl << "All data has been successfully saved to " << fileName << "!" << std::endl;

    return 0;
	/*
	//getting user choice for the algorithm
	std::string userChoice;
	std::cout << "Enter algorithm of choice: ";
	std::cin >> userChoice;

	std::string algorithm = userChoice;
	std::string message = "Hello, world!";

	std::cout << std::endl;

	std::cout << "Algorithm: " << algorithm << std::endl << std::endl;

	KeyManager keymanager(algorithm);
	keymanager.generate_keypair();

	std::cout << "Public key (" << keymanager.public_key_length << " bytes):" << std::endl << keymanager.get_public_key() << std::endl;
	std::cout << std::endl;

	std::cout << "Private key (" << keymanager.private_key_length << " bytes):" << std::endl << keymanager.get_private_key() << std::endl;
	std::cout << std::endl;

	std::cout << "Signing message \"" << message << "\" to get signature (" << keymanager.signature_length << " bytes)" << std::endl;

	unsigned char* signature = keymanager.sign(message);

	// Print the signature
	//std::cout << keymanager.bytes_to_hex(signature, keymanager.signature_length) << std::endl;

	std::cout << std::endl;

	std::cout << "Verifying message and signature: ";

	//message[0]++; // Modify the message to fail verification
	//signature[0]++; // Modify the signature to fail verification

	bool result = keymanager.verify(message, signature);
	if(result) std::cout << "SUCCESS" << std::endl;
	else std::cout << "FAILED" << std::endl;

	std::cout << std::endl;
	*/
	return 0;
}


/* The available algorithms are as follows:

"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/