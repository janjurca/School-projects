/**
 * @file wordcount.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include <stdio.h>
#include "htab.h"
#include "io.h"

#define HTAB_SIZE 9000
///////////////////////////////////////////////////////////////////////////////////
//Testováno nad daty v /usr/share/dict/words (tedy 479828 slov) a hodnota        //
//byla zvolena jako kompromis mezi spotřebou paměti a rychlostí práce s tabulkou //
//nad daným množstvím testovaných dat                                            //
///////////////////////////////////////////////////////////////////////////////////


#define WORD_LENGTH 127

void print(const char * c,int *i){
  printf("%s\t%d\n",c,*i );
}

int main(void) {
  htab_t *tab = htab_init(HTAB_SIZE);
  char word[WORD_LENGTH] = {0};

  int word_length = 0;
  while ((word_length = get_word(word, WORD_LENGTH, stdin)) != EOF) {
    htab_lookup_add(tab, word);
  }
  htab_foreach(tab, print);
  htab_clear(tab);
  htab_free(tab);
  return 0;
}
