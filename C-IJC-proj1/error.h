// error.h
// Řešení IJC-DU1, příklad b), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...);
void error_msg(const char *fmt, ...);

#endif
