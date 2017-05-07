/**
 * @file htab_bucket_count.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * @brief file with function bucket_count
 * FIT VUTBR
 * gcc version 6.3.1
 */

#include "htab_lib.h"

/**
 * return size of htab array
 * @param  t ptr to table
 * @return   size of array
 */
size_t htab_bucket_count(htab_t * t){ // vrátí počet prvků pole (arr_size)
  return t->arr_size;
}
