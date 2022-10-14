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

   int samples = 1; //How long do you want each function to run in seconds?

   //these time variables are needed to control loop length
   
   auto loop_start = std::chrono::steady_clock::now();
   auto loop_end = std::chrono::steady_clock::now();
   auto func_start = std::chrono::steady_clock::now();
   auto func_end = std::chrono::steady_clock::now();
   double loop_time = 0;
   double elapsed_time = 0;
   string fileName_1 = "outputFiles/RSA_keygenTime.csv";
   string fileName_2 = "outputFiles/RSA_signTime.csv";
   string fileName_3 = "outputFiles/RSA_verifyTime.csv";
   ofstream outputFile1;
   ofstream outputFile2;
   ofstream outputFile3;
   outputFile1.open(fileName_1);
   int count = 0;
   
   system("echo Hello World! > myfile.txt");

   cout << "/////////////////// Running RSA4096..." << endl;
   
   //hold for ten seconds to let the system stabilize
   sleep(10);

   cout << "/////// Beginning key generation function: " << endl;

   //begin looping through commands that generate private and public key files
   loop_start = std::chrono::steady_clock::now();
   while(count < samples){
      count++;
      func_start = std::chrono::steady_clock::now();
      system("openssl genrsa -out myprivate.pem 4096 > /dev/null 2>&1");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile1 << elapsed_time << endl;
   }
   loop_end = std::chrono::steady_clock::now();

   cout << "/////// RSA took " << double (std::chrono::duration_cast <std::chrono::microseconds> (loop_end - loop_start).count()) << " microseconds to run key gen " << samples << " times." << endl;

   count = 0;
   outputFile1.close();
   outputFile2.open(fileName_2);

   system("openssl rsa -in myprivate.pem -pubout > mypublic.pem");
   
   //let the system stablize
   sleep(10);	

   cout << "/////// Beginning signing function: " << endl;

   //create the hash and sign
   loop_start = std::chrono::steady_clock::now();
   while(count < samples){
      count++;
      func_start = std::chrono::steady_clock::now();
      system("openssl dgst -sha3-256 -sign myprivate.pem -out sha3-256.sign myfile.txt");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile2 << elapsed_time << endl;
   }
   loop_end = std::chrono::steady_clock::now();

   cout << "/////// RSA took " << double (std::chrono::duration_cast <std::chrono::microseconds> (loop_end - loop_start).count()) << " microseconds to run sign " << samples << " times." << endl;

   count = 0;
   outputFile2.close();
   outputFile3.open(fileName_3);
   
   //let the system stablize
   sleep(10);	
   

   cout << "/////// Beginning verify function: " << endl;

   loop_start = std::chrono::steady_clock::now();
   while(count < samples){
      count++;
      func_start = std::chrono::steady_clock::now();
      system("openssl dgst -sha3-256 -verify mypublic.pem -signature sha3-256.sign myfile.txt > /dev/null 2>&1");
      func_end = std::chrono::steady_clock::now();
      double elapsed_time = double(std::chrono::duration_cast <std::chrono::microseconds> (func_end - func_start).count());
      outputFile3 << elapsed_time << endl;
   }
   loop_end = std::chrono::steady_clock::now();

      
   cout << "/////// RSA took " << double (std::chrono::duration_cast <std::chrono::microseconds> (loop_end - loop_start).count()) << " microseconds to run verify " << samples << " times." << endl;

   //END VERIFY

   count = 0;
   outputFile3.close();

   return 0;
}
