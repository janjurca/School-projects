/**
 * @file tail.c
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define LINE_REALlOC_COUNT 10
typedef struct container {
  char **data;
  long long size;
  long long lines_capacity;
} container;

bool read_to_container(FILE *f, container *cont);
bool init_struct(container *sr);
bool resize_struct(container *sr);
void clear_container(container * cont);

int main(int argc, char const *argv[]) {
  FILE *f;

  int n = 10;
  bool std_in = false;
  if (argc == 1) {
    std_in = true;
  } else if (argc == 2) {
    f = fopen(argv[1], "r");
  } else if (argc == 3) {
    if (strcmp(argv[1], "-n") != 0) {
     fprintf(stderr, "%s %s\n", "Neplatný parametr", argv[1]);
     return -1;
    }
    n = atoi(argv[2]);
    std_in = true;
  } else if (argc == 4){
    if (strcmp(argv[1], "-n") != 0) {
     fprintf(stderr, "%s %s\n", "Neplatný parametr", argv[1]);
     return -1;
    }
    n = atoi(argv[2]);
    f = fopen(argv[3], "r");
  } else {
    fprintf(stderr, "Neplatný počet argumentů\n");
    return -1;
  }

  if (std_in) {
    container cont;
    if(!init_struct(&cont)){
      fprintf(stderr, "%s\n", "Chyba při alokaci dat");
      return -1;
    }
    while (!feof(stdin)) {
      if (!read_to_container(stdin,&cont)) {
        fprintf(stderr, "%s\n", "Chyba pri ukladani dat do kontejneru");
        return -1;
      }
    }
    for (int i = cont.size - n - 1; i < cont.size -1; i++) { // print of result
      if (i < 0) { // if there is not soo much lines as n specifies
        i = 0;
      }
      printf("%s", cont.data[i]);
    }
    clear_container(&cont);
  } else { // cte se z klasickoho souboru
    if(f == NULL){
      fprintf(stderr, "%s\n", "CHYBA při otevírání souboru");
      return -1;
    }
    long long line_count = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
      if (c == '\n') {
        line_count++;
      }
    }
    fseek(f,  0 , SEEK_SET);
    for (long long i = 0; i < (line_count - n); ) {
      if ((c = fgetc(f)) == '\n') {
        i++;
      }
    }
    bool length_overide = false;
    int line_length = 0;
    while ((c = fgetc(f)) != EOF) { //nacitano dat ze souboru
      if (c == '\n') {
        line_length = 0;
      } else {
        line_length++;
      }
      if (line_length > 1024) {
        if(!length_overide){
          fprintf(stderr, "Překročen limit délky řádku\n"); // pri  cteni ze souboru by ani omezeni delky radku byt nemuselo
          length_overide=true;
        }
      } else {
        printf("%c",c );
      }
    }
    fclose(f);
  }
  return 0;
}
/**
 * push line to container
 * @param  f    FILE
 * @param  cont container
 * @return      true/false
 */
bool read_to_container(FILE *f, container *cont){
  static bool errored = false;
  cont->data[cont->size][MAX_LINE_LENGTH-1] = '-';
  fgets(cont->data[cont->size], MAX_LINE_LENGTH, f);

  // test for line length_overide
  if (cont->data[cont->size][MAX_LINE_LENGTH-1] == '\0') {
    if (!errored) {
      fprintf(stderr, "%s\n", "Prekrocena maximalni delka radku");
      errored = true;
    }
  }

  cont->size++;
  // taking care about struct resizing
  if (cont->size == cont->lines_capacity) {
    if (!resize_struct(cont)) {
        fprintf(stderr,"chyba alokace");
      return false;
    }
  }
  return true;
}

/**
 * initialize container
 * @param  sr ptr to container
 * @return    true/false
 */
bool init_struct(container *sr){
  sr->size = 0;
  sr->data = (char**) malloc(sizeof(char*) * LINE_REALlOC_COUNT);
  sr->lines_capacity = LINE_REALlOC_COUNT;
  if (sr->data == NULL) {
    return false;
  }
  for (size_t i = 0; i < LINE_REALlOC_COUNT; i++) {
    sr->data[i] = (char*) calloc(sizeof(char), MAX_LINE_LENGTH);
    if (sr->data[i] == NULL) {
      fprintf(stderr, "%s\n", "Chyba alokace");
      return false;
    }
  }
  return true;
}
/**
 * add new lines to container
 * @param  sr ptr to struct
 * @return    true/false
 */
bool resize_struct(container *sr){
    size_t size = sizeof(char*) * (sr->size + LINE_REALlOC_COUNT);
    sr->data = (char**) realloc(sr->data, size);
    if (sr->data == NULL)
        return false;

    for (int i = sr->lines_capacity; i < ( sr->lines_capacity + LINE_REALlOC_COUNT); i++) {
      sr->data[i] = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);
      if (sr->data[i] == NULL) {
        fprintf(stderr, "%s\n", "Chyba alokace");
        return false;
      }
    }
    sr->lines_capacity += LINE_REALlOC_COUNT;
    return true;
}
/**
 * clear_container
 * @param cont ptr to container
 */
void clear_container(container * cont){
  for (long long i = 0; i < cont->lines_capacity; i++) {
    free(cont->data[i]);
  }
  free(cont->data);
}
