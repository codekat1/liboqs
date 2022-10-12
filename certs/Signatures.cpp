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


#define LOOP_TIME 10
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



// Return the header row for the CSV file
string benchmarkLogHeader() {
	string row = "";
	row += "Name,";
	row += "Public Key Length (bytes),";
	row += "Private Key Length (bytes),";
	row += "Signature Length (bytes),";
	row += "Public+Signature (bytes),";
	row += "Number of samples,";
	row += "Sample input message,";
	row += "Initialization milliseconds,";
	row += "Key generation milliseconds,";
	row += "Signing milliseconds,";
	row += "Verifying milliseconds,";
	return row;
}

// Given an algorithm name, run a benchmark n times and return its CSV row
string benchmarkLog(string algorithm, int n) {
	string message = "Hello, World!";

	double clocks_initialization = 0;
	double clocks_keypair_generation = 0;
	double clocks_signing = 0;
	double clocks_verifying = 0;

	for(int i = 0; i < n; i++) {

		unsigned char* signature;
		bool result;
		char c;

    	SignatureManager sigmanager(algorithm);

		time_t start_time;
		time_t end_time;
		auto func_start = std::chrono::steady_clock::now();
		auto func_end = std::chrono::steady_clock::now();
		double elapsed_time = 0;
		double total_time = 0;
		double avg_time = 0;
		int count = 0;
		string fileName_1 = "outputFiles/" + algorithm + "_keygenTime.csv";
		string fileName_2 = "outputFiles/" + algorithm + "_signTime.csv";
		string fileName_3 = "outputFiles/" + algorithm + "_verifyTime.csv";
		ofstream outputFile1;
		ofstream outputFile2;
		ofstream outputFile3;
		
		outputFile1.open(fileName_1);
	
		//hold for ten seconds to let the system stabilize
		sleep(10);

		//BEGIN KEYGEN

		time(&end_time);
		time(&start_time);
		
		cout << "/////// Beginning key generation function: " << endl;
		cout << "/////// Start time (sec): " << start_time << endl;
		
		while((end_time - start_time) < LOOP_TIME){
			count++;
			func_start = std::chrono::steady_clock::now();
			sigmanager.generate_keypair();
			func_end = std::chrono::steady_clock::now();
			double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
			total_time = total_time + elapsed_time;
			outputFile1 << elapsed_time << endl;
			time(&end_time);
		}

		avg_time = total_time / count;

		cout << "/////// End time (sec): " << end_time << endl;
		cout << "/////// Average length of time for 1 run of key generation: " << avg_time << " microseconds" << endl;
		cout << "/////// Number of times function was run: " << count << endl << "///////" << endl;
		
		count = 0;
		total_time = 0;
		avg_time = 0;
		outputFile1.close();
		outputFile2.open(fileName_2);
		
		//END KEYGEN
		//let systsem stablize
		sleep(10);
		
		//BEGIN SIGN

		time(&start_time);

		cout << "/////// Beginning signing function: " << endl;
		cout << "/////// Start time (sec): " << start_time << endl;

		while((end_time - start_time) < LOOP_TIME){
			count++;
			func_start = std::chrono::steady_clock::now();
			signature = sigmanager.sign(message);
			func_end = std::chrono::steady_clock::now();
			double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
			outputFile2 << elapsed_time << endl;
			total_time = total_time + elapsed_time;
			time(&end_time);
		}
		avg_time = total_time / count;
		
		cout << "/////// End time (sec): " << end_time << endl;
		cout << "/////// Average length of time for 1 run of sign: " << avg_time << " microseconds" << endl;
		cout << "/////// Number of times function was run: " << count << endl << "///////" << endl;;

		count = 0;
		total_time = 0;
		avg_time = 0;
		outputFile2.close();
		outputFile3.open(fileName_3);

		//END SIGN
		//let systsem stablize for 5 seconds
		sleep(10);
		
		//BEGIN VERIFY
		
		time(&start_time);

		cout << "/////// Beginning verify function: " << endl;
		cout << "/////// Start time (sec): " << start_time << endl;

		while((end_time - start_time) < LOOP_TIME){
			count++;
			func_start = std::chrono::steady_clock::now();
			result = sigmanager.verify(message, signature);
			func_end = std::chrono::steady_clock::now();
			double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
			outputFile3 << elapsed_time << endl;
			total_time = total_time + elapsed_time;
			time(&end_time);
		}

		avg_time = total_time / count;
		cout << "/////// End time (sec): " << end_time << endl;
		cout << "/////// Average length of time for 1 run of sign: " << avg_time << " microseconds" << endl;
		cout << "/////// Number of times function was run: " << count << endl << endl;

		//END VERIFY
		
		count = 0;
		total_time = 0;
		avg_time = 0;
		outputFile3.close();
	}



	// Used to quickly grab the key and signature lengths
    SignatureManager sigmanager(algorithm);


	string row = "";
	//row += "\"" + algorithm + "\",";
	//row += to_string(sigmanager.public_key_length) + ",";
	//row += to_string(sigmanager.private_key_length) + ",";
	//row += to_string(sigmanager.signature_length) + ",";
	//row += to_string(sigmanager.public_key_length + sigmanager.signature_length) + ",";
	//row += to_string(n) + ",";
	//row += "\"" + message + "\",";
	//row += to_string(ms_initialization) + ",";
	//row += to_string(ms_keypair_generation) + ",";
	//row += to_string(ms_signing) + ",";
	//row += to_string(ms_verifying) + ",";
	return row;
}

int main(int argc, char** argv) {

	int numSamples = 1;

	string fileName = "Algorithm_benchmark_" + to_string(numSamples) + ".csv";

	ofstream outputFile;
	outputFile.open(fileName);

	outputFile << benchmarkLogHeader() << endl;
	
	
	const char *availAlgs[] = {
		"Dilithium2",
		"Dilithium3", 
		"Dilithium5",
		"Falcon-512",
		"Falcon-1024",
		"SPHINCS+-SHA256-128s-robust",
		"SPHINCS+-SHA256-192s-robust",
		"SPHINCS+-SHA256-256s-robust"
	};
	const int numberOfAlgorithms = sizeof(availAlgs) / sizeof(availAlgs[0]);
    
    for (int i = 0; i < numberOfAlgorithms; i++) {
		string algorithm = availAlgs[i];
		cout << "/////////////////////// Running " << algorithm << endl << "///////" << endl;
    	try {
			//cout << "Progress: " << (100 * (i + 1) / float(numberOfAlgorithms)) << "%" << endl;
			string row = benchmarkLog(algorithm, numSamples);
			outputFile << row << endl;
		} catch(const std::exception& e) {
			cout << "!!!!!!!!!!!!!!!!!!!! ERROR " << algorithm << " does not work." << endl;
        	cout << "Caught exception \"" << e.what() << "\"\n";
		}
    }

    outputFile.close();
    //cout << endl << "All data has been successfully saved to " << fileName << "!" << endl;

    
	return 0;
}


/* The available algorithms are as follows:

"DEFAULT","DILITHIUM_2","DILITHIUM_3","DILITHIUM_4","Falcon-512","Falcon-1024","MQDSS-31-48","MQDSS-31-64","Rainbow-Ia-Classic","Rainbow-Ia-Cyclic","Rainbow-Ia-Cyclic-Compressed","Rainbow-IIIc-Classic","Rainbow-IIIc-Cyclic","Rainbow-IIIc-Cyclic-Compressed","Rainbow-Vc-Classic","Rainbow-Vc-Cyclic","Rainbow-Vc-Cyclic-Compressed","SPHINCS+-Haraka-128f-robust","SPHINCS+-Haraka-128f-simple","SPHINCS+-Haraka-128s-robust","SPHINCS+-Haraka-128s-simple","SPHINCS+-Haraka-192f-robust","SPHINCS+-Haraka-192f-simple","SPHINCS+-Haraka-192s-robust","SPHINCS+-Haraka-192s-simple","SPHINCS+-Haraka-256f-robust","SPHINCS+-Haraka-256f-simple","SPHINCS+-Haraka-256s-robust","SPHINCS+-Haraka-256s-simple","SPHINCS+-SHA256-128f-robust","SPHINCS+-SHA256-128f-simple","SPHINCS+-SHA256-128s-robust","SPHINCS+-SHA256-128s-simple","SPHINCS+-SHA256-192f-robust","SPHINCS+-SHA256-192f-simple","SPHINCS+-SHA256-192s-robust","SPHINCS+-SHA256-192s-simple","SPHINCS+-SHA256-256f-robust","SPHINCS+-SHA256-256f-simple","SPHINCS+-SHA256-256s-robust","SPHINCS+-SHA256-256s-simple","SPHINCS+-SHAKE256-128f-robust","SPHINCS+-SHAKE256-128f-simple","SPHINCS+-SHAKE256-128s-robust","SPHINCS+-SHAKE256-128s-simple","SPHINCS+-SHAKE256-192f-robust","SPHINCS+-SHAKE256-192f-simple","SPHINCS+-SHAKE256-192s-robust","SPHINCS+-SHAKE256-192s-simple","SPHINCS+-SHAKE256-256f-robust","SPHINCS+-SHAKE256-256f-simple","SPHINCS+-SHAKE256-256s-robust","SPHINCS+-SHAKE256-256s-simple","picnic_L1_FS","picnic_L1_UR","picnic_L3_FS","picnic_L3_UR","picnic_L5_FS","picnic_L5_UR","picnic2_L1_FS","picnic2_L3_FS","picnic2_L5_FS","qTesla-p-I","qTesla-p-III"
*/
