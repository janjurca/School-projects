/**
 * @file io.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */

#include "io.h"
/**
 * Function read one word from file to buffer s, word is defined as characters without space
 * @param  s   buffer for wordreding
 * @param  max size of buffer
 * @param  f   FILE
 * @return     size of word
 */
int get_word(char *s, int max, FILE *f){
  int index = 0;
  int c = 0;
  bool reading_word = false;
  while ((c = fgetc(f)) != EOF) {
    if (isspace(c)) {
      if (reading_word == false) { // step over whitespaces
        continue;
      } else {
        if (index >= max) { // for correction of last '\0' byte
          fprintf(stderr, "%s\n", "Prekrocena delka slova");
          s[max - 1] = '\0';
        } else {
          s[index] = '\0';
        }
        return (index >= max) ? max-1 : index; // return  lenght of word
      }
    }
    reading_word = true;
    if (index >= max) {
      continue;
    }
    s[index++] = c;

  }
  return EOF;
}
