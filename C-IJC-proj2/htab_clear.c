/**
 * @file htab_clear.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */

#include "htab_lib.h"

/**
 * clear the table and dealloc items
 * @param t pointer to table
 */
void htab_clear(htab_t * t){ // zrušení všech položek, tabulka zůstane prázdná
  for (size_t i = 0; i < t->arr_size; i++) {
    htab_listitem_t *item = t->item[i];
    while (item != NULL) {
      htab_listitem_t *tmp_item = item;
      item = item->next;
      free(tmp_item->key);
      free(tmp_item);
    }
    t->item[i] = NULL;
  }
  t->arr_size = 0;
  t->n = 0;
}
