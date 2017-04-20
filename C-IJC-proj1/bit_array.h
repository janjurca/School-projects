// bit_array.c
// Řešení IJC-DU1, příklad a), 15.3.2017
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)


#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include "error.h"
#include <limits.h>

typedef unsigned long bit_array_t[];

#define OUT_OF_RANGE(index,mez) error_msg("Index %lu mimo rozsah 0..%lu",(unsigned long)index, (unsigned long)mez)

#define ULONG_BIT_COUNT ((sizeof(unsigned long)) * CHAR_BIT)
#define TYPE_BIT_COUNT(pole) (sizeof(*pole) * CHAR_BIT)

#define ARRAY_SIZE(size)    (size % ULONG_BIT_COUNT > 0) ? ((size / ULONG_BIT_COUNT) + 2) : ((size / ULONG_BIT_COUNT) + 1)
// vytvori pole , inicializuje ho a na [0] index ulozi jeho velikost
#define ba_create(jmeno_pole,velikost) unsigned long jmeno_pole[ARRAY_SIZE(velikost)] = { [0] = velikost,0 }

// pomocne makro na nastaveni bitu
#define BA_SET_BIT_(jmeno_pole,index,vyraz) (vyraz != 0 ) ? (jmeno_pole[1 + (index / TYPE_BIT_COUNT(jmeno_pole))] |= (1UL << (index % TYPE_BIT_COUNT(jmeno_pole)))) : (jmeno_pole[1 + (index / TYPE_BIT_COUNT(jmeno_pole))] &= ~(1UL << (index % TYPE_BIT_COUNT(jmeno_pole))))

// pomocne makro na ziskani hodnoty bitu
#define BA_GET_BIT_(pole,index) (((pole[1 + (index/TYPE_BIT_COUNT(pole))] & (1UL << (index % TYPE_BIT_COUNT(pole)))) > 0) ? 1 : 0)

#ifndef USE_INLINE

//vrati velikost pole v bitech
#define ba_size(jmeno_pole) jmeno_pole[0]

// nastav bit na indexu na výraz (nulový výraz == bit 0, nenulový výraz == bit 1) //TODO volani error funkce
#define ba_set_bit(jmeno_pole,index,vyraz) ((index > ba_size(jmeno_pole)) ? OUT_OF_RANGE(index,ba_size(jmeno_pole)),0 : BA_SET_BIT_(jmeno_pole,index,vyraz))

// získá hodnotu zadaného bitu, vrací hodnotu 0 nebo 1 //TODO volani error funkce
#define ba_get_bit(jmeno_pole,index) ((index > ba_size(jmeno_pole)) ? (OUT_OF_RANGE(index,ba_size(jmeno_pole)),0) : (BA_GET_BIT_(jmeno_pole,index) ?  1 : 0) )

#endif
#ifdef USE_INLINE

static inline unsigned long ba_size(bit_array_t jmeno_pole){
  return jmeno_pole[0];
}

static inline bool ba_set_bit(bit_array_t jmeno_pole,unsigned long index,int vyraz){
    if(index > ba_size(jmeno_pole)){
      OUT_OF_RANGE(index,ba_size(jmeno_pole));
      return false;
    } else {
      BA_SET_BIT_(jmeno_pole,index,vyraz);
      return true;
    }
}

static inline int ba_get_bit(bit_array_t jmeno_pole,unsigned long index){
  if(index > ba_size(jmeno_pole)){
    OUT_OF_RANGE(index,ba_size(jmeno_pole));
  } else {
    if(BA_GET_BIT_(jmeno_pole,index)){
      return 1;
    } else {
      return 0;
    }
  }
  return 0;
}

#endif

#endif
