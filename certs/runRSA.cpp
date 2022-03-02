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

using namespace std;
int main() {

   int LOOP_TIME = 600; //How long do you want each function to run in seconds?

   //these time variables are needed to control loop length
   time_t start_time;
   time_t end_time;
   time(&start_time);
   time(&end_time);
   
   //hold for ten seconds to let the system stabilize
   sleep(10);

   cout << "BEG KEYGEN... CLEAR DATA " << start_time << endl;

   //begin looping through commands that generate private and public key files
   while((end_time - start_time) < LOOP_TIME){
      system("openssl genrsa -out myprivate.pem 1024 > /dev/null 2>&1");
      time(&end_time);
   }
   
   
   system("openssl rsa -in myprivate.pem -pubout > mypublic.pem");
   cout << "END KEYGEN... EXPORT & SAVE FILE " << end_time << endl;
   
   //hold for 60 seconds to export and save file
   sleep(60);	

   time(&start_time);

   cout << "BEG SIGN... CLEAR DATA " << start_time << endl;

   //create the hash and sign
   while((end_time - start_time) < LOOP_TIME){
      system("openssl dgst -sha1 -sign myprivate.pem -out sha1.sign myfile.txt");
      time(&end_time);
   }
   cout << "END SIGN... EXPORT AND SAVE FILE " << end_time << endl;

   
   //hold for 60 seconds to export and save file
   sleep(60);	
   
   time(&start_time);
   cout << "BEG VERIFY... CLEAR DATA " << start_time << endl;
   while((end_time - start_time) < LOOP_TIME){
      system("openssl dgst -sha1 -verify mypublic.pem -signature sha1.sign myfile.txt > /dev/null 2>&1");
      time(&end_time);
   }
   cout << "END VERIFY... EXPORT & SAVE FILE " << end_time << endl;


   return 0;
}
