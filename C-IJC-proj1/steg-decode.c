// steg-decode.c
// Řešení IJC-DU1, příklad a), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)


#include "ppm.h"
#include <stdlib.h>
#include <ctype.h>
#include "eratosthenes.h"
#include "bit_array.h"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    error_msg("Neplatný počet argumenů programu.\n");
  }
  const char *filename = argv[1];


  struct ppm *ppm_struct = ppm_read(filename);
  if (ppm_struct == NULL) {
    return -1;
  }

  ba_create(primes, (1000*1000*3 + 1));
  Eratosthenes(primes);

  char pole[(1000*1000*3)/CHAR_BIT] = {0};
  int pocitadlo = 0;

  for (unsigned long i = 0; i < 3*ppm_struct->xsize*ppm_struct->ysize; i++) {
    if(!ba_get_bit(primes, (i))){
      pole[pocitadlo/CHAR_BIT] |= (((1 << 0) & ppm_struct->data[i]) << pocitadlo%CHAR_BIT); // zapise jeden bbit na konkretní pozici podle toho co je v souboru
      pocitadlo++;
    }
    if(pocitadlo != 0 && (pocitadlo % CHAR_BIT == 7)){
      if (pole[pocitadlo/CHAR_BIT] == '\0') {
        break;
      }
      if (!isprint(pole[pocitadlo/CHAR_BIT])) {
        free(ppm_struct);
        error_msg("Zpráva obsahuje netisknutelné znaky");
      }
    }
  }
  if (pole[pocitadlo/CHAR_BIT] != '\0') {
    free(ppm_struct);
    error_msg("Zpráva není korektně ukončena");
  }
  printf("%s\n",pole);

  ppm_write(ppm_struct, "test.ppm");
  free(ppm_struct);
  return 0;
}
