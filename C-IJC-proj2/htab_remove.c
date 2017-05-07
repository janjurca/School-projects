/**
 * @file htab_remove.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"

/**
 * remove item from table
 * @param  t   table
 * @param  key key to remove
 * @return     true/false
 */
bool htab_remove(htab_t * t, char * key){ // vyhledání a zrušení zadané položky vrací b==false pokud neexistuje
  unsigned int index = (hash_function(key) % t->arr_size);
  htab_listitem_t *item = t->item[index];
  htab_listitem_t *tmp_item = item;
  if (strcmp(item->key, key) == 0) {// handle first bucket
    t->item[index] = item->next;
    free(item->key);
    free(item);
    t->n--;
    return true;
  }
  item = item->next;
  while (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      tmp_item->next = item->next;
      free(item->key);
      free(item);
      t->n--;
      return true;
    }
    tmp_item = item;
    item = item->next;
  }
  return false;
}
