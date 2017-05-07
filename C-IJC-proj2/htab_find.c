/**
 * @file htab_find.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */

#include "htab_lib.h"

/**
 * find item in table
 * @param  t   ptr to table
 * @param  key finding key
 * @return     ptr to item or NULL
 */
struct htab_listitem * htab_find(htab_t *t, const char *key){
  unsigned int index = (hash_function(key) % t->arr_size);
  htab_listitem_t *item = t->item[index];
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      return item;
    }
    item = item->next;
  }
  return NULL;
}
