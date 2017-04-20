// error.c
// Řešení IJC-DU1, příklad b), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#include "error.h"

void warning_msg(const char *fmt, ...){
  va_list args;
  va_start (args, fmt);
  fprintf(stderr,"CHYBA: ");
  vfprintf (stderr,fmt, args);
  va_end (args);
}

void error_msg(const char *fmt, ...){
  va_list args;
  va_start (args, fmt);
  fprintf(stderr,"CHYBA: ");
  vfprintf (stderr,fmt, args);
  va_end (args);
  exit(1);
}
