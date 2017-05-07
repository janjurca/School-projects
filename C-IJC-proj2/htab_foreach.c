/**
 * @file htab_foreach.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"

/**
 * make some action defined by ptr to func with every item in table
 * @param t    ptr to table
 * @param func ptr to func
 */
void htab_foreach(htab_t *t,void (*func)(char const * key,int *value)){ //volání funkce func pro každý prvek
  for (size_t i = 0; i < t->arr_size; i++) {
    htab_listitem_t *item = t->item[i];
    while (item != NULL) {
      func(item->key,&item->data);
      item = item->next;
    }
  }
}
