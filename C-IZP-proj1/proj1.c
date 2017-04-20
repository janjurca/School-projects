/*
 * autor: Jan Jurča - xjurca08
 * vytvořeno jako první projekt do předmětu IZP
 * datum vytvoření: 14.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

void tiskni_poradi(int poradi);
void vypis_pole(char pole[], int pole_size);
void to_hex_formated(int start, int number);
int porovnej_char(char c1[], char c2[]);
int get_size_char_array(char c[]);
void print_hex(int c);
void correct_string_print(int min_delka);
int hex_to_int(char hex[]);
void zobraz_napovedu();

int main(int argc, char *argv[])
{
    int s_flag = 0;
    int n_flag = 0;
    int S_flag = 0;
    int r_flag = 0;
    int x_flag = 0;
    int h_flag = 0;

    //------------------nacitani prepinacu-----------------
    for(int i = 1; i < argc; i++){
        if(porovnej_char(argv[i], "-s")){
            s_flag = i;
        } else if(porovnej_char(argv[i], "-n") ){
            n_flag = i;
        } else if(porovnej_char(argv[i], "-S") ){
            S_flag = i;
        } else if(porovnej_char(argv[i], "-r") ){
            r_flag = i;
        } else if(porovnej_char(argv[i], "-x") ){
            x_flag = i;
        } else if(porovnej_char(argv[i], "-h") ){
            h_flag = i;
        } else if (isdigit(*argv[i])) {

        } else {          
            fprintf(stderr,"Neočekávaný parametr na pozici %i, konkrétně: %s \n",i,argv[i]);
            return 1;
        }
    }
    if(h_flag > 0){
        zobraz_napovedu();
        return 0;
    }
    //---------------zpracovani reakci na prepinace--------
    if(argc < 2){ //bez argumentu
        to_hex_formated(0,-1);
    } else if((s_flag > 0 && n_flag > 0) && (S_flag == 0 && r_flag == 0 && x_flag == 0)){ // s argumentem n a s
        if((argc - 1) > s_flag && (argc - 1) > n_flag ){ //kontrola jestli je zadano dost argumentu na to aby tam mohlo byt u s nebo u n cislo
            if(isdigit(*argv[s_flag + 1]) && isdigit(*argv[n_flag + 1])){
                long s = strtol(argv[s_flag + 1],NULL,10);
                long n = strtol(argv[n_flag + 1],NULL,10);
                to_hex_formated(s,n);
            } else {
                fprintf(stderr,"U parametru s nebo n nebylo zadáno číslo \n");
            }
        } else {
            fprintf(stderr,"Zadán nedostatečný počet parametrů \n");
            return 5;
        }
    } else if(s_flag > 0 && ( n_flag == 0 && S_flag == 0 && r_flag == 0)){ // s argumentem s
        if((argc - 1) > s_flag){ //kontrola dostatecneho poctu argumentu
            if(isdigit(*argv[s_flag + 1])){
                long s = strtol(argv[s_flag + 1],NULL,10);
                to_hex_formated(s,-1);
            } else {
                fprintf(stderr,"po parametru s neni zadane korektni cislo na pozici %i \n", s_flag + 1);
                return 3;
            }
        } else {
            fprintf(stderr,"Nedostatečný počet parametrů\n");
            return 4;
        }
    } else if(n_flag > 0 && (s_flag == 0 && S_flag == 0 && r_flag == 0)){ // s argumentem n
        if((argc - 1) > n_flag){// kontrola dostatecneho poctu argumentu
            if(isdigit(*argv[n_flag + 1])){
                long n = strtol(argv[n_flag + 1],NULL,10);
                to_hex_formated(0,n);
            } else {
                fprintf(stderr,"po parametru n neni zadane korektni cislo na pozici %i \n", n_flag + 1);
                return 2;
            }
        } else {
            fprintf(stderr,"Nedostatečný počet parametrů\n");
            return 5;
        }
    } else if(x_flag > 0 && (s_flag == 0 && n_flag == 0 && S_flag == 0 && r_flag == 0)){ // s argumentem x
        int vstup;
        while((vstup = getchar()) != EOF){
            print_hex(vstup);
        }
    } else if(S_flag > 0 && (s_flag == 0 && n_flag == 0 && x_flag == 0 && r_flag == 0)){ // s argumentem S
        if((argc - 1) > S_flag){
            long S = strtol(argv[S_flag + 1],NULL,10);
            correct_string_print(S);
        } else {
            printf("Nedostatečný počet parametrů \n");
        }
    } else if(r_flag > 0 && (s_flag == 0 && n_flag == 0 && x_flag == 0 && S_flag == 0)){ // s argumentem r
        int vstup;
        char pole_vstup[2];
        int i = 0;
        while ((vstup = getchar()) != EOF) {
            if( isprint(vstup) && !isblank(vstup)){ //naplnovani pole potrebneho pro prevod do int
                pole_vstup[i] = vstup;
                i++;
            }
            if(i == 2){
                printf("%c", hex_to_int(pole_vstup));
                i = 0;
            }
        }
    } else { //spatne zadane argumenty -> zobraz napovedu
        zobraz_napovedu();
        //printf("nezadany argument %i %i %i %i %i", s_flag, n_flag, S_flag, r_flag, error);
    }
    if(S_flag == 0){
        printf("\n");
    }
    return 0;
}
// funkce ktera tiskne pouze korektní řetezce, korektni retezec je definovan minimalni delkou znaků
void correct_string_print(int min_delka){
    char buffer[min_delka]; // pole pro uchovani prvních par znaku dokud ještě nevíme jestli je retezec ok nebo ne
    int i = 0;
    char vstup;
    bool je_correct = true;
    while ((vstup = getchar()) != EOF ) {
        if(i < min_delka && (isprint(vstup) || isblank(vstup))){//cte znak ze vstupu a ukalda ho do pole
            buffer[i] = vstup;
            je_correct = true;
            i++;
        } else if(i < min_delka && (!isprint(vstup) && !isblank(vstup))) {// pokud je retezec kratsi nez byl definovan tak ho přeskoc
            i = 0;
            je_correct = false;
        } else if( i > min_delka && (!isprint(vstup) && !isblank(vstup))){// pokud je retezec dostatecne dlouhy a skoncil tak odradkuj
            printf("\n");
            i = 0;
        } else if( i == min_delka && (!isprint(vstup) && !isblank(vstup))){// pokud je retezec dlouhy akorát a skoncil tak odradkuj
            if(je_correct == true){
                vypis_pole(buffer, min_delka);
                je_correct = false;
            }
            printf("\n");
            i = 0;
        } else {// pokud je retezec dostatecne dlouhy tak ho znak po znaku dovypisuj
           if(je_correct == true){
               vypis_pole(buffer, min_delka);
               je_correct = false;
           }
           putchar(vstup);
        }

    }
}
//tisk cisla v hex podobe
void tiskni_poradi(int poradi){
    printf("%08x  ", poradi);
}
//vypise pole o dane velikosti do printf
void vypis_pole(char pole[], int pole_size){
    for(int i = 0; i < pole_size;i++){
        printf("%c", pole[i]);
    }    
}
// obdoba strcmp zjednodusena
int porovnej_char(char c1[], char c2[]){
    int size_c1 = get_size_char_array(c1);
    int size_c2 = get_size_char_array(c2);
    if(size_c1 == size_c2){
        for(int i = 0; i < size_c1; i++){
            if(c1[i] != c2[i]){
                return 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}
//zjisti velikost pole znaku
int get_size_char_array(char c[]){
    int i = 0;
    while (c[i] != '\0') {
        i++;
    }
    return i;        //printf("%i |%iK ",i, vstup);
}

void print_hex(int c){
    printf("%02x",c);
}
// funkce pro spusteni bez parametru
void to_hex_formated(int start, int number){
    char pole_vstupu[16];
    int sloupec = 1;
    int vstup;
    int poradi = 0;
    bool tisknu = false;
    for(int i = 0; i < start; i++){ // osetreni když neni co tisknout, pri zadani argumentů -s -n
        if( getchar() != EOF){
            poradi++;
            tisknu = true;
        } else {
            tisknu = false;
        }
    }
    while((vstup = getchar()) != EOF){
        tisknu = true;
        if(sloupec == 1){ //tisk sloupce s adresami prvniho znaku
            tiskni_poradi(poradi);
            sloupec++;
        }
        if (sloupec > 1 && sloupec < 18){ // tisk samotnych hex hodnot
            printf("%02x ", vstup);
            if(isprint(vstup)) //zapis do pole vstupu ktere se pouzije v posledni sloupci
                pole_vstupu[sloupec - 2] = vstup;
            else
                pole_vstupu[sloupec - 2] = '.';
            if(sloupec == 9){ //osetreni mezery uprostred
                printf(" ");
            }
            sloupec++;
            poradi++;
            if(number >= 0 && number == (poradi - start)){ // osetreni pro argumenty -n -s
                break;
            }
        }
        if (sloupec == 18){
            printf(" |");
            vypis_pole(pole_vstupu, 16);
            printf("|\n");
            sloupec = 1;
        }
    }

    if(tisknu == false){
        printf("%08x", 0);
    }
    while(sloupec < 18 && sloupec > 1){ // dotiskuni do konce radku prazdnymi mezerami a vypisem pole znaku
        if(sloupec == 9){ //osetreni mezery uprostred
            printf(" ");
        }
        printf("   ");
        pole_vstupu[sloupec - 2] = ' ';
        sloupec++;
        if(sloupec == 18){
            if(tisknu == true){
                printf(" ");
            }
            printf("|");
            vypis_pole(pole_vstupu, 16);
            printf("|");
        }
    }
}

int hex_to_int(char hex[]){
    int number = (int)strtol(hex, NULL, 16); //preved hex a dec int
    return number;
}

void zobraz_napovedu(){
    printf("Program slouží k převodu binárních dat na hexidecimální hodnoty. \n"
           "Syntax spuštění:\n"
           "./proj1 - převod binárních dat do hex\n"
           "./proj1 [-s M] [-n N] - převod binárních dat do hex\n "
           "    -s M - M definuje, na které adrese má výpis začínat\n"
           "    -n N - definuje maximální délku vstupních bajtů ke zpracování. N je kladné číslo.\n"
           "./proj1 -x - Veškerá vstupní data budou převedena do hexadecimální podoby na jeden řádek. \n"
           "./proj1 -S N - Program bude tisknout pouze takové posloupnosti v binárním vstupu, které vypadají jako textový řetězec. \n"
           "    -S N - N je minimální délka řetězce\n"
           "./proj1 -r - Převod hex dat na textový výstup\n");
}
