//===================================================================================================================================
// semiprimesieve.c: Sieve for all semiprimes <= x.
//===================================================================================================================================
// Author: Simon Goater Jan 2026
// Uses a naive sieve to get all semiprimes <= x < 2^32. 
// RAM requirement > 5x bytes.
// Allocate and set arrays counts[1+x] and products[1+x] to all 0s and 1s respectively.
// For i = 1 to pi(isqrt(x))
//   primepower = p_i
//   while (primepower <= x)
//     primepowermultiple = primepower
//     while (primepowermultiple <= x)
//       counts[primepowermultiple]++
//       products[primepowermultiple] *= p_i
//       primepowermultiple += primepower
//     primepower *= p_i
// Semiprimes are indices s >= 4 where (counts[s] = 2 && products[s] = s)
// or (counts[s] = 1 && products[s] < s)
//
// COPYRIGHT NOTICE: Copying, modifying, and distributing with conspicuous attribution for any purpose is permitted.
//===================================================================================================================================

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "/home/simon/Mairsonsprimesieve.c" // https://github.com/FastAsChuff/Primes-List/blob/main/Mairsonsprimesieve.c

// gcc semiprimesieve.c -o semiprimesieve.bin -O3 -Wall -mssse3 -lm

uint32_t isqrt(uint64_t n) {
  if (n < 2) return n;
  uint64_t ai = sqrt(n);
  while (!((ai <= n/ai) && ((ai+1) > n/(ai+1)))) {    
    ai = (ai + n/ai)/2;
  }
  return ai;
}

int main(int argc, char* argv[]) {
  uint32_t x = 1000;
  if (argc > 1) {
    x = atol(argv[1]);
  } 
  char *justtotalstext = "--justtotals";
  _Bool justtotals = false;
  if (argc > 2) {
    justtotals = (0 == strcmp(justtotalstext, argv[2]));
  }
  if ((x < 4) || (argc <= 1)) {
    printf("This program prints all semiprimes less than or equal to 4 <= x < 2^32.\nUsage: %s x [%s]\nRAM requirement > 5x bytes.\nAuthor: Simon Goater Jan 2026\n", argv[0], justtotalstext);
    exit(0);
  }
  if (x < 4) exit(0);
  uint32_t isqrtx = isqrt(x);
  uint32_t numprimes;
  uint32_t *primes = Mairsonsprimesieve(isqrtx, &numprimes);
  assert(primes);  
  uint32_t numsemiprimes = 0;
  uint64_t sumsemiprimes = 0;
  uint32_t *products = malloc((1+(uint64_t)x)*sizeof(uint32_t));
  assert(products);
  for (uint32_t i=1; ;i++) {
    products[i] = 1;
    if (i == x) break;
  }
  uint8_t *counts = calloc((1+(uint64_t)x), sizeof(uint8_t));
  assert(counts);
  for (uint32_t i=0; i<numprimes; i++) {
    uint64_t primepower = primes[i];
    while (primepower <= x) {
      uint64_t primepowermultiple = primepower;
      while (primepowermultiple <= x) {
        counts[primepowermultiple]++;
        products[primepowermultiple] *= primes[i];
        primepowermultiple += primepower;
      }
      primepower *= primes[i];
    }
  }
  for (uint32_t i=4; ;i++) {
    if (((counts[i] == 2) && (products[i] == i)) || ((counts[i] == 1) && (products[i] < i))) {
      if (!justtotals) printf("%u\n", i);
      numsemiprimes++;
      sumsemiprimes += i;
    }
    if (i == x) break;
  }
  printf("Found %u semiprimes with sum %lu in [1, %u].\n", numsemiprimes, sumsemiprimes, x);
  free(primes);
  free(counts);
  free(products);  
}
// x=10^n
// n=1 Found 4 semiprimes with sum 29.
// n=2 Found 34 semiprimes with sum 1707.
// n=3 Found 299 semiprimes with sum 146158.
// n=4 Found 2625 semiprimes with sum 12736914.
// n=5 Found 23378 semiprimes with sum 1138479765.
// n=6 Found 210035 semiprimes with sum 102604509687.
// n=7 Found 1904324 semiprimes with sum 9322298311255.
// n=8 Found 17427258 semiprimes with sum 854762170949394.
// n=9 Found 160788536 semiprimes with sum 79000703096467991.
