/**
 * @file io.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * @brief interface of library
 * FIT VUTBR
 * gcc version 6.3.1
 */
#ifndef  HTAB_LIB_H
#define HTAB_LIB_H value

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct htab_listitem {
    char *key;
    int data;
    struct htab_listitem *next;
} htab_listitem_t;

typedef struct htab htab_t;


extern unsigned int hash_function(const char *str);
extern htab_t * htab_init(size_t size);
extern htab_t * htab_move(int newsize,htab_t * t2);
extern size_t htab_size(htab_t * t); //vrátí počet prvků tabulky (n)
extern size_t htab_bucket_count(htab_t * t);  // vrátí počet prvků pole (arr_size)
extern void htab_foreach(htab_t *t,void (*func)(char const * key,int *value)); //volání funkce func pro každý prvek
extern bool htab_remove(htab_t * t, char * key); // vyhledání a zrušení zadané položky vrací b==false pokud neexistuje
extern void htab_clear(htab_t * t); // zrušení všech položek, tabulka zůstane prázdná
extern void htab_free(htab_t * t);  // destruktor: zrušení tabulky (volá htab_clear())
extern struct htab_listitem * htab_lookup_add(htab_t *t, const char *key);
extern struct htab_listitem * htab_find(htab_t *t, const char *key);

#endif
