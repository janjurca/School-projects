/**
 * @file htab_size.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"

/**
 * return count of items in table
 * @param  t table
 * @return   size
 */
size_t htab_size(htab_t * t){ //vrátí počet prvků tabulky (n)
  return t->n;
}
