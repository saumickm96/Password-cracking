#include <stdio.h>
#include <cuda_runtime_api.h>

/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
  
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __global__ functions
    2) Enable a simulation of password cracking in the absence of library 
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
    nvcc -o Crack_with_CUDA Crack_with_CUDA.cu
    ./Crack_with_CUDA
   
  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is 
  identical to the plain text password string stored in the program. 
  Otherwise,it returns 0.
*****************************************************************************/

__device__ int is_a_match(char *attempt) {
  char plain_password[] = "KB7867";
  
  char *a = attempt;
  char *p = plain_password;
  
  while(*a == *p) {
    if(*a == '\0') {
      return 1;
    }
    a++;
    p++;
  }
  return 0;
}

/*********************************￼*******************************************
  The kernel function assume that there will be only one thread and uses 
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

__global__ void  kernel() {
  
  int i, j, k, l; 
  char c;
  char password[7];
  
  password[0] = 'A' + threadIdx.x;
  password[1] = 'A' + blockIdx.x;
  
  for(i=0; i<10; i++) {
    c = i+'0';
     password[2] = c;
      
	for(j=0; j<10; j++) {
         c = j+'0';
          password[3] = c;
  
 		for(k=0; k<10; k++) {
                 c = k+'0';
                  password[4] = c;
  

 			for(l=0; l<10; l++) {
     			 c = l+'0';
     			  password[5] = c;
                            password[6] = '\0';

      if(is_a_match(password)) {
         printf("password found: %s, Thread id is %d\n", password);
        } 
      else {
      //printf("tried: %s\n", password);		  
          }
        }
      }
    }
  }
}

int time_difference(struct timespec *start, 
                    struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec;
  
  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
 
}
	

int main() {

  struct timespec start, finish;   
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

//Launch the kernel  
kernel<<<26, 26>>>();
cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));


  return 0;
}
