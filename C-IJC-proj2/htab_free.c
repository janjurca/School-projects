/**
 * @file htab_free.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"
/**
 * dealloc the table
 * @param t table
 */
void htab_free(htab_t * t){ // destruktor: zrušení tabulky (volá htab_clear())
  htab_clear(t);
  free(t);
}
