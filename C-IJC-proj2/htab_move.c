/**
 * @file htab_move.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"

/**
 * make new table and fill it with old data and clear old tab
 * @param  newsize size of new table
 * @param  t2      old table
 * @return         ptr to new tab
 */
htab_t * htab_move(int newsize,htab_t * t2){
  htab_t *new_tab = htab_init(newsize);
  if (new_tab == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < t2->arr_size; i++) {
    htab_listitem_t *item = t2->item[i];
    while (item != NULL) {
      htab_listitem_t *item_ptr = htab_lookup_add(new_tab, item->key);
      if (item_ptr == NULL) {
        fprintf(stderr, "Chyba při vkládání prvku s klíčem \"%s\" do nové tabulky",item->key);
      }
      item = item->next;
    }
  }
  htab_clear(t2);
  return new_tab;
}
