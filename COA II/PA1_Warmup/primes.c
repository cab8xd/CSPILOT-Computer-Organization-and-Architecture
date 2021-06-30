//
//  primes.c
//  
//
//  Created by Christine Baca on 1/22/19.
//

#include "primes.h"


//Test cases:
/*
    check for 0, 1, 2
 */


/* isprime
 * Accepts a single integer and returns 1 if it is prime, 0 otherwise.
 * You are welcome to brute-force this or use the primality checking from COA1’s smallc program.
 */

int isprime(int n){
    
    if(n == 1)
        return 1;
    
    else if(n == 2)
        return 1;
    
    else if(n % 2 != 0){
        // A number x is prime if and only
        // if it is 2, or it is odd and is not a multiple of any odd number between 3 and √x.
        int m = n * n;
        int o = 9;
        while(o <= m)
        {
            if(n % o == 0)
                return 0;
            o += 2;
        }
        return 1;
    }
    
    return 0;
}




/* nextprime
 * See COA1’s smallc for a description.
 */

int nextprime(int n){
    int c = n + 1;
    while(! (isprime(c)) )
    {
        c += 1;
    }
    return c;
}

/* pprime
 *  Given an integer, print a single line to standard out containing either
 * (number) is prime
 or
* (number) is not prime, but (bigger number) is
 where (number) is the argument integer and (bigger number) is the result of invoking nextprime on the argument
 */

void pprime(int n)
{
    if( isprime(n) )
        printf("%d is prime\n", n);
    else
        printf("%d is not prime, but %d is\n",n,nextprime(n));
}
