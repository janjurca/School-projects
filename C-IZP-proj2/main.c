/*
 * autor: Jan Jurča - xjurca08
 * vytvořeno jako druhý projekt do předmětu IZP
 * datum vytvoření: 14.10.2016)
 * OBSAHUJE BONUS
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define EPS 0.000000000001
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
void show_help();
void arg_error();
double mylog(double x);
double mypow(double x, double y);
bool isNumeric(const char *str);
int array_size(char *array);

int main(int argc, char **argv)
{

    int log_flag = 0;
    int pow_flag = 0;
    int mylog_flag = 0;
    int mypow_flag = 0;

    int h_flag = 0;

    for(int i = 1; i < argc; i++){//vyhledání argumentů
        if(strcmp(argv[i], "--log") == 0){
            log_flag = i;
        } else if(strcmp(argv[i], "--pow") == 0 ){
            pow_flag = i;
        } else if(strcmp(argv[i], "--mypow") == 0 ){
            mypow_flag = i;
        } else if(strcmp(argv[i], "--mylog") == 0 ){
            mylog_flag = i;
        } else if(strcmp(argv[i], "-h") == 0){
            h_flag = i;
        } else if (isNumeric(argv[i])) {

        } else {
            fprintf(stderr,"Neočekávaný parametr na pozici %i, konkrétně: %s \n",i,argv[i]);
            return 0;
        }
    }
    if(h_flag > 0){ // vypis napovedu
        show_help();
        return 0;
    }

    if(argc < 2){
        fprintf(stderr, "Nedostatečný počet argumentů \n");
    } else if (log_flag > 0 && pow_flag == 0 && h_flag == 0 && mylog_flag == 0 && mypow_flag == 0) { // volání funkcí logaritmů
        if(argc == (log_flag + 3)){//testovaní dostatecného poctu argumenů
            if(isNumeric(argv[log_flag + 1]) && isNumeric(argv[log_flag + 2])){// kontrola správného formátu argumentů
                double x = strtod(argv[log_flag + 1],NULL);
                double n = strtod(argv[log_flag + 2],NULL);
                if(n <= 0){ //ověření hodnoty počtů iterací
                    fprintf(stderr,"Chybný argument iterace\n");
                    return 1;
                }
                printf("       log(%g) = %.12g\n",x,log(x));
                printf(" cfrac_log(%g) = %.12g\n",x,cfrac_log(x,n));
                printf("taylor_log(%g) = %.12g\n",x,taylor_log(x,n));
            } else {
                arg_error();
                return 1;
            }
        } else {
            arg_error();
            return 1;
        }
    } else if (pow_flag > 0 && log_flag == 0 && h_flag == 0 && mylog_flag == 0 && mypow_flag == 0) { // volání funkcí mocniny
        if(argc == (pow_flag + 4)){ // test dotatečného počtu argumentů
            if(isNumeric(argv[pow_flag + 1]) && isNumeric(argv[pow_flag + 2]) && isNumeric(argv[pow_flag + 3])){ // kontrola správného formátu arguentů
                double x = strtod(argv[pow_flag + 1],NULL);
                double y = strtod(argv[pow_flag + 2],NULL);
                double n = strtod(argv[pow_flag + 3],NULL);
                if(n <= 0){ // oveěření hodnoty počtu iterací
                    fprintf(stderr,"Chybný argument iterace\n");
                    return 1;
                }
                printf("         pow(%g,%g) = %.12g\n",x,y,pow(x,y));
                printf("  taylor_pow(%g,%g) = %.12g\n",x,y,taylor_pow(x,y,n));
                printf("taylorcf_pow(%g,%g) = %.12g\n",x,y,taylorcf_pow(x,y,n));
            } else {
                arg_error();
                return 1;
            }
        } else {
            arg_error();
            return 1;
        }
    } else if (mylog_flag > 0 && log_flag == 0 && h_flag == 0 && pow_flag == 0 && mypow_flag == 0) { // volání funkcí mylog
        if(argc == (mylog_flag + 2)){// test dostatecneho poctu argumentu
            if(isNumeric(argv[mylog_flag + 1])){ // test spravnosti argumentu
                double x = strtod(argv[mylog_flag + 1],NULL);
                printf("       log(%g) = %.7e\n",x,log(x));
                printf("     mylog(%g) = %.7e\n",x,mylog(x));
            } else {
                arg_error();
                return 2;
            }
        } else {
            arg_error();
            return 3;
        }
    } else if (mypow_flag > 0 && log_flag == 0 && h_flag == 0 && pow_flag == 0 && mylog_flag == 0) { // volání funkcí mypow
        if(argc == (mypow_flag + 3)){ // kontrola poctu argumentů
            if(isNumeric(argv[mypow_flag + 1]) && isNumeric(argv[mypow_flag + 2])){ //kontrola spravnosti argumentu
                double x = strtod(argv[mypow_flag + 1],NULL);
                double y = strtod(argv[mypow_flag + 2],NULL);
                double mypow_d = mypow(x,y);
                printf("         pow(%g,%g) = %.7e\n",x,y,pow(x,y));
                printf("       mypow(%g,%g) = %.7e\n",x,y,mypow_d);
            } else {
                arg_error();
                return 1;
            }
        } else {
            arg_error();
            return 1;
        }
    } else {
        arg_error();
        return 1;
    }
    return 0;
}

/**
 * @brief taylor_log
 * @param x cislo ze ktereho se pocita prirozeny logaritmus
 * @param n pocet iteraci
 * @return vraci spocitanou hodnotu (pomoci taylorova polynomu) prirozeneho logaritmu z cisla x
 */
double taylor_log(double x, unsigned int n){
    if(x < 0){ // oseterni definicniho oboru logaritmu
        return NAN;
    } else if(x == 0){
        return -INFINITY;
    } else if(isinf(x)) {
        return INFINITY;
    } else if(isnan(x)) {
        return NAN;
    }else {
        double log = 0;
        if(x < 1.0 && x > 0.0){ // vzorec pro 0 < x < 1
            x = 1.0 - x;
            log = x * -1;
            double x_puvodni = x;

            for(int i = 0; i < (int)n; i++){
                x = x * x_puvodni;
                log -= x /(i + 2);
            }
            return log;
        } else if(x > 1.0){ // vzorec pro x > 1
            double vyraz_citatel = ((x - 1)/x);
            double citatel = vyraz_citatel;
            for(int i = 1; i <= (int)n; i++){
                log += citatel/i;
                citatel *= vyraz_citatel;
            }
            return log;
        } else {
            return 0;
        }
    }
}
/**
 * @brief cfrac_log
 * @param x cislo ze ktereho se pocita prirozeny logaritmus
 * @param n pocet iteraci
 * @return vraci spocitanou hodnotu (pomoci zretezeneho zlomku) prirozeneho logaritmu z cisla x
 */
double cfrac_log(double x, unsigned int n){
    if(x < 0){ // osetereni definicniho oboru logaritmu
        return NAN;
    } else if(x == 0){
        return -INFINITY;
    } else if(isinf(x)) {
        return INFINITY;
    } else if(isnan(x)) {
        return NAN;
    } else {
        x =((x - 1.0) / (x + 1.0));
        double cf = 0;
        double z_sqr = x * x;
        for(int i = n; i > 0; i--){
            double liche_cislo = i * 2 + 1;
            double konstanta_u_z = i * i;
            cf = (konstanta_u_z * z_sqr) / (liche_cislo - cf);
        }
        cf = 1 - cf;
        double log = (2 * x) / cf;
        return log;
    }
}

/**
 * @brief taylorcf_pow
 * @param x základ exponentu
 * @param y exponent
 * @param n počet iterací výpočtu
 * @return pow vrací hodnotu vypočítané mocniny (pomocí taylorova polynomu a zřetězeného zlomku)
 */
double taylorcf_pow(double x, double y, unsigned int n){
    if (x == 1) {// osetreni defnicnich oboru
           return 1;
    }else if (y == 0) {
        return 1;
    } else if(isnan(x) || isnan(y)){
        return NAN;
    } else if(x == 0 && y > 0){
        return 0;
    } else if ( x == 0 && y < 0){
        return INFINITY;
    } else if(y == 0){
        return 1;
    } else  if(isinf(x) && y > 0){
        return INFINITY;
    } else  if(isinf(x) && y < 0){
        return 0;
    }
    double pow = 1;
    double zlomek = 1;
    double ln_log = cfrac_log(x,n);
    for(int i = 1; i <= (int)n; i++){
        zlomek = zlomek * ((y * ln_log)/i);
        pow += zlomek;
    }
    return pow;
}
/**
 * @brief taylor_pow
 * @param x základ exponentu
 * @param y exponent
 * @param n počet iterací výpočtu
 * @return pow vrací hodnotu vypočítané mocniny (pomocí taylorova polynomu)
 */
double taylor_pow(double x, double y, unsigned int n){
    if (x == 1) { // osetreni defnicnich oboru
           return 1;
    }else if (y == 0) {
        return 1;
    } else if(isnan(x) || isnan(y)){
        return NAN;
    } else if(x == 0 && y > 0){
        return 0;
    } else if ( x == 0 && y < 0){
        return INFINITY;
    } else if(y == 0){
        return 1;
    } else  if(isinf(x) && y > 0){
        return INFINITY;
    } else  if(isinf(x) && y < 0){
        return 0;
    }    double pow = 1;
    double zlomek = 1;
    double ln_log = taylor_log(x,n);
    for(int i = 1; i <= (int)n; i++){
        zlomek = zlomek * ((y * ln_log)/i);
        pow += zlomek;
    }
    return pow;
}

/**
 * @brief arg_error
 */
void arg_error(){
    fprintf(stderr, "Chyba v argumentech \n");
}

/**
 * @brief show_help
 */
void show_help(){
    printf("Program pro výpočet přirozeného logaritmu a mocniny\n"
           "--log X N požadavek pro výpočet přirozeného logaritmu z čísla X v N iteracích (Taylorova polynomu a zřetězeného zlomku).\n"
           "--pow X Y N požadavek pro výpočet exponenciální funkce z čísla Y s obecným základem X v N iteracích (Taylorova polynomu a zřetězeného zlomku)\n");
}

/**
 * @brief isNumeric
 * @param str ukazatel na retezec k otestovani zdali je to ciste cislo nebo ne
 * @return vraci true v případě že dané pole reprezentuje číselnou hodnotu
 */
bool isNumeric(const char *str)
{
    char *nptr_strtod;//vstupni pole ke kontrole
    strtod(str, &nptr_strtod);
    if(array_size(nptr_strtod) > 0){//pokud v poli neco bude znamena to ze tam byly nejaka pismena
        return false;
    }else return true;

}

/**
 * @brief array_size
 * @param array ukazatel na pole
 * @return vrací hodnotu velikosti pole
 */
int array_size(char *array){
    int i = 0;
    for(; array[i] != '\0';i++){}
    return i;
}

/**
 * @brief mylog
 * @param x
 * @return vrací hodnotu spočítaného logaritmu
 */
double mylog(double x){
    if(x < 0){ // oseterni definicniho oboru logaritmu
        return NAN;
    } else if(x == 0){
        return -INFINITY;
    } else {
        double log;
        if(x < 1.0 && x > 0.0){// implementace vzorce pro 0 < x < 1
            x = 1.0 - x;
            log = x * -1;
            double x_puvodni = x;
            double vyraz = 1;
            int i = 0;
            while(fabs(vyraz) > EPS){
                x = (x * x_puvodni);
                vyraz = x /(i + 2);
                log -= vyraz;
                i++;
            }
            return log;
        } else if(x > 1.0){ // vzorec pro x > 1
            double vyraz_citatel = ((x - 1)/x);
            double citatel = vyraz_citatel;
            double vyraz = 1;
            int i = 1;
            while(fabs(vyraz) > EPS){
                vyraz = citatel/i;
                log += vyraz;
                citatel *= vyraz_citatel;
                i++;
            }
            return log;
        } else {
            return 0;
        }
    }
}

/**
 * @brief mypow
 * @param x základ
 * @param y exponent
 * @return vrací spočítanou hodnotu mocniny
 */
double mypow(double x, double y){
    double pow = 1;
    double zlomek = 1;
    int i = 1;
    double ln_log = mylog(x);
    double zlomek_puvodni = 0;
    while(fabs(zlomek - zlomek_puvodni) > 0.00001){
        zlomek_puvodni = zlomek;
        zlomek = zlomek * ((y * ln_log)/i);
        pow += zlomek;
        i++;
    }
    return pow;
}
