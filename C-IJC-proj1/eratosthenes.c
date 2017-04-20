// eratosthenes.c
// Řešení IJC-DU1, příklad a), 18.3.2017
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#include "eratosthenes.h"

void Eratosthenes(bit_array_t pole){
  unsigned long bits_to_count = sqrt(ba_size(pole)); //ba_size(pole);
  if(bits_to_count > 1){
    ba_set_bit(pole, 0, 1);
    ba_set_bit(pole, 1, 1);
    unsigned long arr_size = ba_size(pole);

    for(unsigned long int i = 2; i <= bits_to_count; i++ ){
      if(ba_get_bit(pole, i) == 0){ //TODO opravit na inline funkci
        for(unsigned long int j = i*2; j <= arr_size; j += i){
          ba_set_bit(pole, j, 1);
        }
      }
    }
  }
}
