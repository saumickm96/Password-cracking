 #include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <crypt.h>

#include <time.h>

 

/******************************************************************************

  Demonstrates how to crack an encrypted password using a simple

  "brute force" algorithm. Works on passwords that consist only of 2 uppercase

  letters and a 2 digit integer. Your personalised data set is included in the

  code.

 

  Compile with:

    cc -o CrackAZ99-With-Data CrackAZ99-With-Data.c -lcrypt

 

  If you want to analyse the results then use the redirection operator to send

  output to a file that you can view using an editor or the less utility:

 

    ./CrackAZ99-With-Data > results.txt

 

  Dr Kevan Buckley, University of Wolverhampton, 2018

******************************************************************************/

int n_passwords = 4;

 

char *encrypted_passwords[] = {

  "$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",

  "$6$KB$Tla1KqvXDinHtPrvPktPT8M9aoC.qfkk9jBpbfX4K/JBaJ3c3JpUcHbMAye4/NfZUkXZqD6f/p6OiXqE9aUzj/",

  "$6$KB$bYBwYK5aXh/NvrFF8sc8m6.NyXS5macytgp6d1GojayV0gkl41OEJA3iyraj5CABameIrFEWEXU2O6FqxSWj41",

  "$6$KB$DGeluDMo8WEHfbXuxcFl6tILZaF6dGBxiSdYnco0BH2KfFxr5zLhbZoEhYGqtJ1NYlLRgq.dAjHa4eAA54KdO0"

};

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

  int count = 0;   // The number of combinations explored so far

 

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

  printf("%d solutions explored\n", count);

}

int time_difference(struct timespec *start, 
                    struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec;
}

int main(int argc, char *argv[]){

  int i;


  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
   
  
  for(i=0;i<n_passwords;i<i++) {

    crack(encrypted_passwords[i]);

  }


  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
                                         (time_elapsed/1.0e9));


return 0;

}




 
