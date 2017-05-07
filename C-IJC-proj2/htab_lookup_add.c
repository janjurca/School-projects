/**
 * @file io.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include "htab_lib.h"

/**
 * add the item to table
 * @param  t   table
 * @param  key key
 * @return     ptr to item
 */
struct htab_listitem * htab_lookup_add(htab_t *t, const char *key){
  unsigned int index = (hash_function(key) % t->arr_size);
  htab_listitem_t *item = t->item[index];
  htab_listitem_t *prev_item = item;
  if (item == NULL) { //head of list init
    item =  (htab_listitem_t*) malloc(sizeof(htab_listitem_t));
    if (item == NULL) {
      return NULL;
    }
    int length = strlen(key);
    item->key = (char*) malloc(length + 1); // +1 because of '\0'
    if (item->key == NULL) {
      return NULL;
    }
    strcpy(item->key, key);
    item->data = 1;
    item->next = NULL;
    t->item[index] = item;
    t->n++;
    return item;
  } // end of list init

  while (item != NULL) { // find of existing element
    if (strcmp(item->key, key) == 0) {
      item->data++;
      return item;
    }
    prev_item = item;
    item = item->next;
  }
  // item haven't been found soo we make another one;
  prev_item->next =(htab_listitem_t*) malloc(sizeof(htab_listitem_t));
  if (prev_item->next == NULL) {
    return NULL;
  }
  int length = strlen(key);
  prev_item->next->key = (char*) malloc(length + 1); // +1 because of '\0'
  if (prev_item->next->key == NULL) {
    return NULL;
  }
  strcpy(prev_item->next->key, key);
  prev_item->next->data = 1;
  prev_item->next->next = NULL;
  t->n++;
  return prev_item->next;

}
