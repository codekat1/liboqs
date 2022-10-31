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

#define LOOP_TIME 120 //How long do you want each function to run in seconds?
int main() {

   //these time variables are needed to control loop length
   
   time_t start_time;
   time_t end_time;
   
   system("echo Hello World! > myfile.txt");

   cout << "/////////////////// Running RSA4096..." << endl;
   
   //hold for ten seconds to let the system stabilize
   sleep(10);

   time(&start_time);
   time(&end_time);

   cout << "/////// Keygen start time: " << start_time << " seconds." << endl;

   //begin looping through commands that generate private and public key files
   while((end_time - start_time) < LOOP_TIME){
      system("openssl genrsa -out myprivate.pem 4096 > /dev/null 2>&1");
      time(&end_time);
   }

   cout << "/////// Keygen end time: " << end_time << " seconds." << endl;

   system("openssl rsa -in myprivate.pem -pubout > mypublic.pem");
   
   //let the system stablize
   sleep(10);

   time(&start_time);
	
   cout << "/////// Sign start time: " << start_time << " seconds." << endl;

   //create the hash and sign
   while((end_time - start_time) < LOOP_TIME){
      system("openssl dgst -sha3-256 -sign myprivate.pem -out sha3-256.sign myfile.txt");
      time(&end_time);
   }

   cout << "/////// Sign end time: " << end_time << " seconds." << endl;
   
   //let the system stablize
   sleep(10);	
   
   time(&start_time);
   cout << "/////// Beginning verify function: " << endl;

   while((end_time - start_time) < LOOP_TIME){
      system("openssl dgst -sha3-256 -verify mypublic.pem -signature sha3-256.sign myfile.txt > /dev/null 2>&1");
      time(&end_time);
   }

   cout << "/////// Verify end time: " << end_time << " seconds." << endl;

   //END VERIFY


   return 0;
}
