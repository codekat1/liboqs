//////////////////////////////////////////////////////////////////////////////////////////////////
//Run RSA key generation, signing, and verifying
//This code loops through each function for a pre-specified amount of time
//
//Issue: LINE 28: Cannot surpress output without errors (2/24/2022)

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<ctime>
#include<unistd.h>
#include<fstream>
#include<chrono>

using namespace std;
int main() {

   int LOOP_TIME = 30; //How long do you want each function to run in seconds?

   //these time variables are needed to control loop length
   time_t start_time;
   time_t end_time;
   auto func_start = std::chrono::steady_clock::now();
   auto func_end = std::chrono::steady_clock::now();
   double elapsed_time = 0;
   double total_time = 0;
   double avg_time = 0;
   int count = 1;
   string fileName_1 = "RSA_keygenTime.csv";
   string fileName_2 = "RSA_signTime.csv";
   string fileName_3 = "RSA_verifyTime.csv";
   ofstream outputFile1;
   ofstream outputFile2;
   ofstream outputFile3;
   outputFile1.open(fileName_1);
   
   system("echo Hello World! > myfile.txt");

   cout << "/////////////////// Running RSA4096..." << endl;
   
   //hold for ten seconds to let the system stabilize
   sleep(10);

   time(&start_time);
   time(&end_time);

   cout << "/////// Beginning key generation function: " << endl;
   cout << "/////// Start time (sec): " << start_time << endl;

   //begin looping through commands that generate private and public key files
   while((end_time - start_time) < LOOP_TIME){
      func_start = std::chrono::steady_clock::now();
      system("openssl genrsa -out myprivate.pem 4096 > /dev/null 2>&1");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile1 << elapsed_time << endl;
      total_time = total_time + elapsed_time;
      count++;
      time(&end_time);
   }
   
   avg_time = total_time / count;
   cout << "/////// End time (sec): " << end_time << endl;
   cout << "/////// Average length of time for 1 run of key generation: " << avg_time << " microseconds" << endl;
   cout << "/////// Number of times function was run: " << count << endl << "///////" << endl;
   
   count = 1;
   total_time = 0;
   avg_time = 0;
   outputFile1.close();
   outputFile2.open(fileName_2);

   system("openssl rsa -in myprivate.pem -pubout > mypublic.pem");
   
   //let the system stablize
   sleep(10);	

   time(&start_time);

   cout << "/////// Beginning signing function: " << endl;
   cout << "/////// Start time (sec): " << start_time << endl;

   //create the hash and sign
   while((end_time - start_time) < LOOP_TIME){
      func_start = std::chrono::steady_clock::now();
      system("openssl dgst -sha3-256 -sign myprivate.pem -out sha3-256.sign myfile.txt");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile2 << elapsed_time << endl;
      total_time = total_time + elapsed_time;
      count++;
      time(&end_time);
   }
   
   avg_time = total_time / count;
   cout << "/////// End time (sec): " << end_time << endl;
   cout << "/////// Average length of time for 1 run of sign: " << avg_time << " microseconds" << endl;
   cout << "/////// Number of times function was run: " << count << endl << "///////" << endl;;

   count = 1;
   total_time = 0;
   avg_time = 0;
   outputFile2.close();
   outputFile3.open(fileName_3);
   
   //let the system stablize
   sleep(10);	
   
   time(&start_time);

   cout << "/////// Beginning verify function: " << endl;
   cout << "/////// Start time (sec): " << start_time << endl;

   while((end_time - start_time) < LOOP_TIME){
      func_start = std::chrono::steady_clock::now();
      system("openssl dgst -sha3-256 -verify mypublic.pem -signature sha3-256.sign myfile.txt > /dev/null 2>&1");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile3 << elapsed_time << endl;
      total_time = total_time + elapsed_time;
      count++;
      time(&end_time);
   }
   
   avg_time = total_time / count;
   
   cout << "/////// End time (sec): " << end_time << endl;
   cout << "/////// Average length of time for 1 run of sign: " << avg_time << " microseconds" << endl;
   cout << "/////// Number of times function was run: " << count << endl << endl;

   //END VERIFY

   count = 1;
   total_time = 0;
   avg_time = 0;
   outputFile3.close();

   return 0;
}
