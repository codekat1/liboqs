#include <ctime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <oqs/oqs.h>
#include <unistd.h>
#include <chrono>

using namespace std;

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

	sign(message): Return the signature (unsigned char*) for a message (string)

	verify(message, signature): Return true or false for if a message and signature are valid
*/
class SignatureManager {
private:
	char* algorithm_char;
	OQS_SIG *sig;

public:
	string algorithm;
	unsigned int public_key_length, private_key_length, signature_length;
	unsigned char *public_key, *private_key, *signature = NULL;

	SignatureManager(string _algorithm) {
		algorithm = _algorithm;
		algorithm_char = const_cast<char*>(algorithm.c_str());
		
		if(OQS_SIG_alg_is_enabled(algorithm_char) == 0) {
			throw runtime_error("ERROR: Algorithm \"" + algorithm + "\" does not exist");
		}

		OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
		sig = OQS_SIG_new(algorithm_char);

		public_key_length = sig->length_public_key;
		private_key_length = sig->length_secret_key;
		signature_length = sig->length_signature;


		public_key = (unsigned char*) malloc(public_key_length);
		*public_key = 0;
		private_key = (unsigned char*) malloc(private_key_length);
		*private_key = 0;
		signature = (unsigned char*) malloc(signature_length);
		*signature = 0;
	}

	~SignatureManager(){
		free(public_key);
		free(private_key);
		free(signature);
	}

	// Generate a public and private key pair
	void generate_keypair() {
		OQS_STATUS status = OQS_SIG_keypair(sig, public_key, private_key);
		if(status != OQS_SUCCESS) throw runtime_error("ERROR: OQS_SIG_keypair failed\n");
	}

	// Get the generated public key
	string get_public_key() {
		return bytes_to_hex(public_key, public_key_length);
	}

	// Get the generated private key
	string get_private_key() {
		return bytes_to_hex(private_key, private_key_length);
	}

	// Sign a message, returns its signature
	unsigned char* sign(string message) {
		//size_t *signature_len = (size_t*) &signature_length;
		unsigned int message_length = message.length();
		unsigned char message_bytes[message_length];
		strcpy( (char*) message_bytes, message.c_str());
		//uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		size_t temp_siglen = (size_t) signature_length;
		OQS_STATUS status = OQS_SIG_sign(sig, signature, &temp_siglen, message_bytes, message_length, private_key);

		if (status != OQS_SUCCESS) throw runtime_error("ERROR: OQS_SIG_sign failed\n");

		return signature;
	}

	// Verify a signature
	bool verify(string message, unsigned char* signature) {
		unsigned int message_length = message.length();
		//size_t *signature_len = (size_t*) &signature_length;
		uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

		OQS_STATUS status = OQS_SIG_verify(sig, message_bytes, message_length, signature, signature_length, public_key);

		return status == OQS_SUCCESS;
	}

	// Given an array of bytes, convert it to a hexadecimal string
	static string bytes_to_hex(unsigned char* bytes, int len) {
		constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		string output(len * 2, ' ');
		for (int i = 0; i < len; ++i) {
			output[2 * i] = hexmap[(bytes[i] & 0xF0) >> 4];
			output[2 * i + 1] = hexmap[bytes[i] & 0x0F];
		}
		return output;
	}
};

// "Dilithium2", "Dilithium3",  "Dilithium5", "Falcon-512", "Falcon-1024", "SPHINCS+-SHA256-128s-robust", "SPHINCS+-SHA256-192s-robust", "SPHINCS+-SHA256-256s-robust"
int main(int argc, char** argv) {
	if(argc < 3) {
		cout << endl;
		cout << "ERROR: Missing required two arguments:" << endl;
		cout << "    ./gdb_operations_experiment.o ALGORITHM [keygen/sign/verify]" << endl;
		cout << endl;
		return 1;
	}
	string algorithm = argv[1];
	string operation = argv[2];

	SignatureManager sigmanager(algorithm);
	unsigned char* signature;
	bool result;
	string message = "Hello, World!";

	if(operation == "keygen") {
		//////////////////////////// Breakpoint at Line 142
		sigmanager.generate_keypair();
		//////////////////////////// Breakpoint at Line 144
	} else if(operation == "sign") {
		sigmanager.generate_keypair();
		//////////////////////////// Breakpoint at Line 147
		signature = sigmanager.sign(message);
		//////////////////////////// Breakpoint at Line 149
	} else if(operation == "verify") {
		sigmanager.generate_keypair();
		signature = sigmanager.sign(message);
		//////////////////////////// Breakpoint at Line 153
		result = sigmanager.verify(message, signature);
		//////////////////////////// Breakpoint at Line 155

	} else {
		cout << endl;
		cout << "ERROR: Invalid second argument:" << endl;
		cout << "    ./gdb_operations_experiment.o ALGORITHM [keygen/sign/verify]" << endl;
		cout << endl;
		return 1;
	}
	return 0;
}


/* The available algorithms are as follows:
"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/



// gdb -q ./gdb_operations_experiment.o -ex "set pagination off" -ex "set print asm-demangle" -ex "disas main" -ex quit | awk '{print $3}' | sort |uniq -c | sort -r | head