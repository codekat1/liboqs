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

OQS_SIG *sig;
unsigned char *public_key, *private_key, *signature;











// Return must be on line 30 for the opcode trace to work
void log() {
	OQS_STATUS status = OQS_SIG_keypair(sig, public_key, private_key);
	//if(status != OQS_SUCCESS) throw runtime_error("ERROR: OQS_SIG_keypair failed\n");
	return; // Line 30
}

// "Dilithium2", "Dilithium3",  "Dilithium5", "Falcon-512", "Falcon-1024", "SPHINCS+-SHA256-128s-robust", "SPHINCS+-SHA256-192s-robust", "SPHINCS+-SHA256-256s-robust"
int main(int argc, char** argv) {
	char* algorithm_char = argv[1];
	if(OQS_SIG_alg_is_enabled(algorithm_char) == 0) {
		throw runtime_error("ERROR: Algorithm does not exist");
	}
	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
	sig = OQS_SIG_new(algorithm_char);

	public_key = (unsigned char*) malloc(sig->length_public_key);
	private_key = (unsigned char*) malloc(sig->length_secret_key);
	//unsigned char *signature = (unsigned char*) malloc(sig->length_signature);

	log();
}

/* The available algorithms are as follows:
"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/
