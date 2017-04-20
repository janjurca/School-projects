// primes.c
// Řešení IJC-DU1, příklad a), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#include <stdio.h>
#include <stdbool.h>
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"

#define TO_PRINT 10

int main() {
  ba_create(pole, 303000000); // inicializace pole
  Eratosthenes(pole); //vytridi prvocisla v poli pole

  unsigned long primes_arr[TO_PRINT] = {0};
  int primes_count = TO_PRINT;

  unsigned long int cycle_counter = ba_size(pole);
  while( primes_count >= 0){
    if(!ba_get_bit(pole, cycle_counter)){
      primes_arr[--primes_count] = cycle_counter;
    }
    cycle_counter -= 1;
  }

  for (size_t i = 0; i < TO_PRINT; i++) {
    printf("%lu\n",primes_arr[i] );
  }

  return 0;
}
