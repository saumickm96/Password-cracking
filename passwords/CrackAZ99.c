#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer.

  Compile with:
    cc -o CrackAZ99 CrackAZ99.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:
    ./CrackAZ99 > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/

int count=0;     // The number of combinations explored so far

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}

int main(int argc, char *argv[]){
  crack("$6$KB$O1fCjJpf3J5VAD.EaKzlSKn7O7ezwCNy7Fo0AoaG.rczR.t5xsHa8h7be9Qly8M/dxmaRwhGJMQvn4AUtD0ZE.");
  printf("%d solutions explored\n", count);

  return 0;
}

