// ppm.h
// Řešení IJC-DU1, příklad b), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#ifndef PPM_H
#define PPM_H

#include <stdio.h>

struct ppm {
   unsigned xsize;
   unsigned ysize;
   char data[];    // RGB bajty, celkem 3*xsize*ysize
};

/*
 * Načte obsah PPM souboru do touto funkcí dynamicky alokované struktury.
 * Při chybě formátu použije funkci warning_msg a vrátí NULL.  Pozor na "memory leaks".
 */
struct ppm * ppm_read(const char * filename);

/*
 * Zapíše obsah struktury p do souboru ve formátu PPM.
 * Při chybě použije funkci warning_msg a vrátí záporné číslo.
 */
int ppm_write(struct ppm *p, const char * filename);

#endif
