// ppm.c
// Řešení IJC-DU1, příklad b), 20.3.2111
// Autor: Jan Jurča, FIT
// Přeloženo: gcc 6.3.1 20161221 (Red Hat 6.3.1-1)

#include "ppm.h"
#include "error.h"

struct ppm * ppm_read(const char * filename){
  FILE *ppm_file = fopen(filename,"r");
  if(ppm_file == NULL){
    warning_msg("Soubor nelze otevřít\n");
    return NULL;
  }

  struct ppm *ppm_struct;

  int ppm_type = 0;
  int ppm_max_value = 0;
  int x = 0;
  int y = 0;

  if(!feof(ppm_file)){ // Nacteni formatu ppm
    if(fscanf(ppm_file, "P%d \n ",&ppm_type) != 1){
      warning_msg("Chybný formát souboru\n");
      fclose(ppm_file);
      return NULL;
    }
    if (ppm_type != 6) {
      warning_msg("Chybný formát souboru\n");
      fclose(ppm_file);
      return NULL;
    }
  }
  if(!feof(ppm_file)){ // Nacteni velikosti ppm
    if(fscanf(ppm_file, "%d %d\n ",&x,&y) != 2){
      warning_msg("Chybný formát souboru\n");
      fclose(ppm_file);
      return NULL;
    }
    if(x * y > 1000000){
      warning_msg("Obrázek je příliš velký\n");
      fclose(ppm_file);
      return NULL;
    }
  }
  if(!feof(ppm_file)){ // Nacteni maximalni hodnoty ppm
    if(fscanf(ppm_file, "%d \n ",&ppm_max_value) != 1){
      warning_msg("Chybný formát souboru\n");
      fclose(ppm_file);
      return NULL;
    }
    if(ppm_max_value !=255){
      warning_msg("Chybný formát souboru\n");
      fclose(ppm_file);
      return NULL;
    }
  }

  // Alokace strukutry
  ppm_struct = (struct ppm*) malloc(sizeof(struct ppm) + 3 * x * y * sizeof(char));
  if(ppm_struct->data == NULL){
    warning_msg("Chyba alokace struktury\n");
    fclose(ppm_file);
    return NULL;
  }
  ppm_struct->xsize = x;
  ppm_struct->ysize = y;


  long char_array_index = 0;
  long char_max_index = 3 * ppm_struct->xsize * ppm_struct->ysize;
  int c = 0;
  int counter = 0;
  while((c = fgetc(ppm_file)) != EOF) { /// načítá jednotlivé bajty dat
    if(char_array_index == char_max_index){
      warning_msg("Chyba v souboru,velké množství dat\n");
      fclose(ppm_file);
      free(ppm_struct);
      return NULL;
    }
    ppm_struct->data[char_array_index++] = c;
    counter++;
  }
  if (counter < (char_max_index-1)) {
    warning_msg("Chyba v souboru,malé množství dat\n");
    fclose(ppm_file);
    free(ppm_struct);
    return NULL;
  }
  fclose(ppm_file);
  return ppm_struct;
}

int ppm_write(struct ppm *p, const char * filename){
  FILE *ppm_file = fopen(filename, "w");
  if(ppm_file == NULL){
    warning_msg("Nelze otevřít soubor pro zápis\n");
    return -1;
  }
  if (p == NULL) {
    warning_msg("Neplatný ukazatel na strkturu\n");
    return -2;
  }

  // Zapis formatu
  fprintf(ppm_file, "P6\n");
  fprintf(ppm_file, "%d %d\n", p->xsize,p->ysize);
  fprintf(ppm_file, "255\n");
  // Zapis dat
  int char_array_size = 3 *  p->xsize * p->ysize;
  for (int i = 0; i <= char_array_size; i++) {
    if(putc(p->data[i], ppm_file) == EOF){
        warning_msg("Chyba při zápisu souboru\n");
        fclose(ppm_file);
        return -1;
    }
  }
  fclose(ppm_file);
  return 1;
}
