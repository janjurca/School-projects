/**
 * @file htab_init.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"
/**
 * initialize the table
 * @param  size size of table
 * @return      ptr to table
 */
htab_t * htab_init(size_t size){
  htab_t *tab = (htab_t *) malloc(sizeof(htab_t) + sizeof(htab_listitem_t) * size);
  if (tab == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < size; i++) {
    tab->item[i] = NULL;
  }
  tab->arr_size = size;
  tab->n = 0;
  return tab;
}
