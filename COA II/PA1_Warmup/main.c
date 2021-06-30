//
//  main.c
//  
//
//  Created by Christine Baca on 1/22/19.
//

#include <stdio.h>
//#include <primes.h>



/*
 
 main.c
 If there are command-line arguments,
 convert each to an integer and invoke pprime with each. I
 
 If there are no command line arguments,
 read integers from standard input (provided in base-10,
 one number per line) until the end of standard input is reached and invoke pprime with each integer.
 
 main should not print anything itself.
 */


int main(int argc, char const *argv[]){
    char buff[4096];
    while(read(0,buff,4096) != 0)
    {
        pprime(strtol(buff,NULL,10));
    }
    
    return 0;
}

