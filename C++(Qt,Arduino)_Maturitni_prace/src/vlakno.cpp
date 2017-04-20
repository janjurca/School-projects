#include "vlakno.h"
#include <QFile>
#include <QDebug>
#include <QtCore>
#include <math.h>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "qcustomplot.h"

#define PI 3.14159265
#define delka_x_kroku 0.009075 // mm
#define delka_y_kroku 0.009075 // mm

double pocet_radku;
double pocet_radku_obnovy;
double procento_postup;
double procento_double;
int procento_int;
int stav; // 0 nic ; 1 x ; 2 y ; 3 z
int counter;
double zz;
double xx;
double yy;
double iii;
int iiii = 0;
double jj;
double z = 15; // parametr rozlišení ------------------------------------------------- cim vic tim "lip"
double x_aktualni , y_aktualni;
QString a , radek, slovo;
QByteArray text;
QString radky;
QString line;
QStringList list,list_1,list_2;
QStringList prichozi_list;
QFile g_code;
QFile x_y_z("out.txt");
//QSerialPort *serial;
QSerialPort serial;
int z_krok_int_ref = 2;
QVector<double> x, y;
QCustomPlot *nahled;

/*! \file */
/**
  * Trida vlakno je pracovní trida, ve které se odehrává cely proces tisku(interpolace bodů, seriova komunikace)
  * @class vlakno
  *
  */


/**
 * @brief vlakno::vlakno
 * @param parent
 */

vlakno::vlakno(QObject *parent):
  QThread(parent)
{


}
/**
 * vlákno provádějící všechny tisknoucí úlohy a výpočty
 * @brief vlakno::run
 */
void vlakno::run(){
    if(x_y_z.isOpen()) x_y_z.close();
    if(g_code.isOpen()) g_code.close();
    QMutex mutex;
    mutex.lock();
    if(filename == NULL){
        mutex.unlock();
        emit stav("Soubor nevybrán");
    }
    else{
        emit stav("Příprava tisku");
        serial.close();
        QMutex mutex_1;
        mutex_1.lock();
        serial.setPortName(port);
        qWarning() << port;
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        if(!serial.open(QIODevice::ReadWrite)){

            qWarning() << serial.errorString();
            emit stav("nelze se připojit k usb");
            return;
        }
        mutex_1.unlock();
        this->sleep(2);

    x_aktualni = 0;
    y_aktualni = 0;
    qWarning() << "filename" << filename;
    g_code.setFileName(filename);
    g_code.open(QIODevice::ReadOnly | QIODevice::Text);
    x_y_z.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&g_code);
    qWarning() << "inicializace znovuotevření vstupu a inicializace vystupu";
    while(!in.atEnd()){
        line = in.readLine();
        pocet_radku++;
        qWarning() << pocet_radku;
    }
    procento_double = 100 / pocet_radku;
    qWarning() << procento_double;
    g_code.close();
    g_code.open(QIODevice::ReadOnly | QIODevice::Text);

       while (!in.atEnd()){
           line = in.readLine();
           list = line.split(" ");
           //qWarning() << list;


           if(list[0] == "G00"){
               //qWarning() << "ZDE JE G00";
               int k = list.size();
               for(int i = 0 ; i < k ; i++){  // začátek rekognoskace xyz zatím rozděleno na slova
               list_1 = list[i].split("");   // rozděleno na písmena
               int j = list_1.size();
               //qWarning() << list_1 << "TADY" << j;

               for(int ii = 0; ii < j;ii++){ // rozpoznávní typu souřadnice zda xyz
                   if(list_1[ii] == "X"){
                       list[i] = list[i].remove("X");
                       xx = list[i].toDouble();
                       //qWarning() << "xx:"<< xx;
                       //qWarning() << list[i] << "\n";

                   }
                   if(list_1[ii] == "Y"){
                       list[i] = list[i].remove("Y");
                       yy = list[i].toDouble();
                       //qWarning() << yy;
                   }
                   if(list_1[ii] == "Z"){
                       list[i] = list[i].remove("Z");
                       zz = list[i].toDouble();
                   }

               }
           }
          this->interpolace_usecky(xx,yy);
       }
           if(list[0] == "G01"){
               //qWarning() << "ZDE JE G01";
               int k = list.size();
               for(int i = 0 ; i < k ; i++){  // začátek rekognoskace xyz zatím rozděleno na slova
               list_1 = list[i].split("");   // rozděleno na písmena
               int j = list_1.size();
               //qWarning() << list_1 << "TADY" << j;

               for(int ii = 0; ii < j;ii++){ // rozpoznávní typu souřadnice zda xyz
                   if(list_1[ii] == "X"){
                       list[i] = list[i].remove("X");
                       xx = list[i].toDouble();
                       //qWarning() << xx;
                       //qWarning() << list[i] << "\n";

                   }
                   if(list_1[ii] == "Y"){
                       list[i] = list[i].remove("Y");
                       yy = list[i].toDouble();
                       //qWarning() << yy;
                   }
                   if(list_1[ii] == "Z"){
                       list[i] = list[i].remove("Z");
                       zz = list[i].toDouble();
                   }

               }
           }
           this->interpolace_usecky(xx,yy);
           }
           if(list[0] == "G02"){
               //qWarning() << "ZDE JE G02";
               int k = list.size();
               for(int i = 0 ; i < k ; i++){  // začátek rekognoskace xyz zatím rozděleno na slova
               list_1 = list[i].split("");   // rozděleno na písmena
               int j = list_1.size();
               //qWarning() << list_1 << "TADY" << j;

               for(int ii = 0; ii < j;ii++){ // rozpoznávní typu souřadnice zda xyz
                   if(list_1[ii] == "X"){
                       list[i] = list[i].remove("X");
                       xx = list[i].toDouble();
                       //qWarning() << xx;
                       //qWarning() << list[i] << "\n";
                   }
                   if(list_1[ii] == "Y"){
                       list[i] = list[i].remove("Y");
                       yy = list[i].toDouble();
                       //qWarning() << yy;
                   }
                   if(list_1[ii] == "I"){

                       list[i] = list[i].remove("I");
                       iii = list[i].toDouble();
                       //qWarning() << iii;
                   }
                   if(list_1[ii] == "J"){
                       list[i] = list[i].remove("J");
                       jj = list[i].toDouble();
                       //qWarning() << jj;
                   }

                   if(list_1[ii] == "Z"){
                       list[i] = list[i].remove("Z");
                       zz = list[i].toDouble();
                       //qWarning() << zz;
                   }

               }
           }
           this->interpolace_kruznice_g02(xx,yy,iii,jj);
       }
           if(list[0] == "G03"){
               //qWarning() << "ZDE JE G03";
               int k = list.size();
               for(int i = 0 ; i < k ; i++){  // začátek rekognoskace xyz zatím rozděleno na slova
               list_1 = list[i].split("");   // rozděleno na písmena
               int j = list_1.size();
               //qWarning() << list_1 << "TADY" << j;

               for(int ii = 0; ii < j;ii++){ // rozpoznávní typu souřadnice zda xyz
                   if(list_1[ii] == "X"){
                       list[i] = list[i].remove("X");
                       xx = list[i].toDouble();
                       //qWarning() << xx;
                       //qWarning() << list[i] << "\n";
                   }
                   if(list_1[ii] == "Y"){
                       list[i] = list[i].remove("Y");
                       yy = list[i].toDouble();
                       //qWarning() << yy;
                   }
                   if(list_1[ii] == "I"){

                       list[i] = list[i].remove("I");
                       iii = list[i].toDouble();
                       //qWarning() << iii;
                   }
                   if(list_1[ii] == "J"){
                       list[i] = list[i].remove("J");
                       jj = list[i].toDouble();
                       //qWarning() << jj;
                   }

                   if(list_1[ii] == "Z"){
                       list[i] = list[i].remove("Z");
                       zz = list[i].toDouble();
                       //qWarning() << zz;
                   }

               }
           }
           this->interpolace_kruznice_g03(xx,yy,iii,jj);
       }
           procento_postup += procento_double;
           procento_int = (int)procento_postup;
           emit postup(procento_int);
       }
       emit postup(100);
       procento_postup = 0;
           g_code.close();
           x_y_z.close();
           x_aktualni = 0;
           y_aktualni = 0;
           xx = 0;
           yy = 0;
           zz = 0;
           pocet_radku = 0;           

           //-------------------nahledy tisku-------------

           nahled = new QCustomPlot();
           // create graph and assign data to it:
           nahled->addGraph();
           nahled->graph(0)->setData(x, y);
           nahled->graph(0)->setLineStyle(QCPGraph::lsNone);
           nahled->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
           nahled->setInteraction(QCP::iRangeZoom, true);
           // give the axes some labels:
           nahled->xAxis->setLabel("x");
           nahled->yAxis->setLabel("y");
           // set axes ranges, so we see all data:
           nahled->xAxis->setRange(0, 400);
           nahled->yAxis->setRange(0, 400);
           nahled->replot();
           nahled->setGeometry(0,0,300,300);
           nahled->show();

    qWarning() << "probíhá tisk-------------------------------------------------------";
    emit stav("Probíhá tisk");

    x_y_z.open(QIODevice::ReadOnly | QIODevice::Text);
    qWarning() << "probíhá tisk-------------------------------------------------------";
    QTextStream in_kroky_radky(&x_y_z);
    while(!in_kroky_radky.atEnd()){
        line = in_kroky_radky.readLine();
        pocet_radku++;
        //qWarning() << pocet_radku;
    }
    procento_double = 100 / pocet_radku;
    //qWarning() << procento_double;
    x_y_z.close();
    x_y_z.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in_kroky(&x_y_z);
    while(!in_kroky.atEnd()){
        //qWarning() << pause;
        while(pause){
            qWarning() << "pozastaveno";
            /*if(x_y_z.isOpen()){
                x_y_z.close();
                qWarning() << "soubor odpojen";
            }*/
            sleep(1);
        }
        if(!x_y_z.isOpen())x_y_z.open(QIODevice::ReadOnly | QIODevice::Text);

        line = in_kroky.readLine();
        int zasilka = line.toInt();
        this->poslat_int(zasilka);
        procento_postup += procento_double;
        procento_int = (int)procento_postup;
        emit postup(procento_int);
    }
    x_y_z.close();
    serial.close();
    this->filename.clear();
    this->odpojit();
    emit stav("Tisk dokončen");
    emit postup(100);
    return;
}

}
/**
 * Posílá na zvolený seriový port posílá číselná data
 *
 * @brief vlakno::poslat_int
 * @param data Cislo k poslani
 * @return int
 */
int vlakno::poslat_int(int data){
   QString data_qstr = QString::number(data);
   const char *data_char = data_qstr.toStdString().c_str();
   qWarning() << "data_char" << data_char;
   //qWarning() << "data int" << data;
   QByteArray responseData;
   //qWarning() << responseData;
   int waitTimeout = 5000;
   serial.write(data_char);
   if (serial.waitForBytesWritten(waitTimeout)) {
       // read response
       if (serial.waitForReadyRead(waitTimeout)){
           responseData = serial.read(1);
           if(responseData == "c"){
               //qWarning() << "odpovida";
               return 1;
           }
       } else  {

           qWarning() <<"neodpovida";
           serial.write(data_char);
           if (serial.waitForBytesWritten(waitTimeout)) {
               // read response
               if (serial.waitForReadyRead(waitTimeout)){
                   responseData = serial.read(1);
                   if(responseData == "c"){
                       //qWarning() << "odpovida";
                       return 1;
                   }
               }
           }

       }
   } else {
       //qWarning() << "Wait write request timeout %1";
   }
}
/**
 * Zapisuje do dodatečného souboru všechny instrukce, které budou následně řádek po řádku poslány do plotteru
 * @brief vlakno::zapsat
 * @param data
 */
void vlakno::zapsat(int data){
    QTextStream out(&x_y_z);
    out << data << "\n";
    //qWarning() << "zapsano";
}
/**
 * Vypočítává kroky pro motory z aktuální a následné pozice
 * @brief vlakno::prevod_na_m_kod
 * @param x_kon
 * @param y_kon
 */
void vlakno::prevod_na_m_kod(double x_kon , double y_kon ){
    qWarning() << "---------------------------------";
    qWarning() << x_kon << ";" << y_kon;
    qWarning() << "x_aktualni" << x_aktualni;
    qWarning() << "y_aktualni" << y_aktualni;
    if(x_kon < 0) x_kon = 0;
    if(y_kon < 0) y_kon = 0;
    double x_posun = x_kon - x_aktualni; // zjistení o kolik se budeme posunovat
    double y_posun = y_kon - y_aktualni; // zjistení o kolik se budeme posunovat
    double x_krok, y_krok;
    if(x_posun >= 0){
        x_krok = (x_posun / delka_x_kroku) + 0.5; // plus 0.5 je to kvuli zaokrouhlovani
    }else {
        x_krok = (x_posun / delka_x_kroku) - 0.5; // plus 0.5 je to kvuli zaokrouhlovani
    }
    if (y_posun >= 0) {
        y_krok = (y_posun / delka_y_kroku) + 0.5;
    } else {
        y_krok = (y_posun / delka_y_kroku) - 0.5;
    }
    //qWarning() << "iiii " << iiii;
    qWarning() << "x_krok" << x_krok;
    qWarning() << "y_krok" << y_krok;

    int x_krok_int = 0;
    int y_krok_int = 0;
    int z_krok_int = 0;
    x_krok_int = (int)x_krok;
    y_krok_int = (int)y_krok;
    if(zz > 0) z_krok_int = 1;
    if(zz <= 0) z_krok_int = 0;

    x << x_kon; // zapis souradnic pro vizualizaci kvuli grafu
    y << y_kon;
    iiii++;

        this->zapsat(601);//x
        this->zapsat(x_krok_int);
        this->zapsat(602);//y
        this->zapsat(y_krok_int);
    if(z_krok_int_ref != z_krok_int){
        this->zapsat(600);//z
        this->zapsat(z_krok_int);
        z_krok_int_ref = z_krok_int;
    }
    }
/**
 * Metoda Interpolujici body přímky, využití u funkcí G01 a G00
 * @brief vlakno::interpolace_usecky
 * @param x_kon
 * @param y_kon
 */
void vlakno::interpolace_usecky( double x_kon, double y_kon) {
/*
  v = konecny_bod - vychozi bod = ( x_v ; y_v )
  x = x_vych + x_v * t
  y = y_vych + y_v * t
*/

    double p;
    double x_vych = x_aktualni;
    double y_vych = y_aktualni;
    double t = 0; // t musí mít hodnoty od  0 do 1 protože jen tyto hodnoty vahodí body ležící na usecce
    int i;
    double x_v = x_kon - x_vych;
    double y_v = y_kon - y_vych;
    //delka usecky
    double delka = qSqrt(x_v*x_v+y_v*y_v);
    double x_01;
    double x_02;
    p = z * (delka/4);
    double t_d = 1/p;
    /*
    qWarning() << "x_kon" << x_kon;
    qWarning() << "y_kon" << y_kon;
    qWarning() << "x_v" << x_v;
    qWarning() << "y_v" << y_v;
    qWarning() << "p" << p;
    qWarning() << "x_akt" << x_aktualni;
    qWarning() << "y_akt" << y_aktualni;
    qWarning() << "t_d" << t_d;

*/

    for(i = 0 ; i < p ; i++){
        t += t_d;
        x_01 = x_vych + x_v * t;
        x_02 = y_vych + y_v * t;
        //qWarning() << x_01 << ";" << x_02;
        prevod_na_m_kod(x_01, x_02);
        x_aktualni = x_01;
        y_aktualni = x_02;
    }
    t = 0;


}
/**
 * Metoda interpolujíci body kruznice pro funkci G03
 * @brief vlakno::interpolace_kruznice_g03
 * @param x_kon
 * @param y_kon
 * @param i
 * @param j
 */
void vlakno::interpolace_kruznice_g03(double x_kon, double y_kon, double i, double j){

    counter++;
   // qWarning() << counter;
    int stopka = 0;
    int kvadrant_odskok;
    int kvadrant_kon = 0;
    int kvadrant_vyp;
    double r, s_x, s_y, a, b, alfa,alfa_k, alfa_skok, alfa_kon_rad, alfa_kon, alfa_skok_rad,alfa_prechodova, x_01, x_02;
    double p;
    alfa = 0;
    alfa_k = 0;
    alfa_kon = 0;
    alfa_kon_rad = 0;
    alfa_skok = 0;
    alfa_skok_rad = 0;

    s_x = x_aktualni + i;
    s_y = y_aktualni + j;  // tady uz mam definovany stred kruznice
    /*ted budu vatvaret vektro S a koncovy bod tedy v = koncovy_bod - S =(x_kon - s_x; y_kon - s_y)
     *a to abych mohl vypocitat velikost r
     */
    //r = qSqrt((y_aktualni - s_x) * (x_aktualni - s_x) + (y_aktualni - s_y) * (y_aktualni - s_y));
    r = qSqrt((i*i)+(j*j));
    //qWarning() << "r:" << r;
    /*jelikoz mame prumer tak muzu urcit z kolika bodu se ta kruznice bude skladat (rozliseni) */
    p = r * z; //  p = mnozstvi bodu tvoricich  kruznici
    alfa = 360 / p; // určení velikosti uhlu v zavislosti na počtu vzorkovacích bodu
    alfa_k = alfa; // alfa_k ke velikost uhlu jednoho kroku
    //qWarning() << "p: "<< p << " alfa_k: " << alfa_k << "\n";

    if(y_aktualni >= s_y && x_aktualni >= s_x){   // vypocet odskokoveho pro 1. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((y_aktualni-s_y)/(x_aktualni - s_x));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = alfa_skok;
    kvadrant_odskok = 1;
    }
    if(y_aktualni > s_y && x_aktualni < s_x){   // vypocet odskokoveho pro 2. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((s_x - x_aktualni)/(y_aktualni-s_y));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = alfa_skok + 90;
    kvadrant_odskok = 2;
    }
    if(y_aktualni <= s_y && x_aktualni <= s_x){   // vypocet odskokoveho pro 3. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((s_y - y_aktualni)/(s_x - x_aktualni));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = alfa_skok + 180;
    kvadrant_odskok = 3;
    }
    if(y_aktualni < s_y && x_aktualni > s_x){   // vypocet odskokoveho pro 4. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((s_x - x_aktualni)/(s_y - y_aktualni));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = alfa_skok + 270;
    kvadrant_odskok = 4;
    }

    if(y_kon >= s_y && x_kon >= s_x){   // vypocet konecneho pro 1. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((y_kon - s_y)/(x_kon - s_x));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    kvadrant_kon = 1;
    }
    if(y_kon > s_y && x_kon < s_x){   // vypocet konecneho pro 2. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((s_x - x_kon)/(y_kon - s_y));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon += 90;
    kvadrant_kon = 2;
    }
    if(y_kon <= s_y && x_kon <= s_x){   // vypocet konecneho pro 3. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((s_y - y_kon)/(s_x - x_kon));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon += 180;
    kvadrant_kon = 3;
    }
    if(y_kon < s_y && x_kon > s_x){   // vypocet konecneho pro 4. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((x_kon - s_x)/(s_y - y_kon));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon += 270;
    kvadrant_kon = 4;
    }
/*
    qWarning() << "G03";
    qWarning() << "----------------------------------------------------";
    qWarning() << "x_aktual" << x_aktualni;
    qWarning() << "y_aktual" << y_aktualni;
    qWarning() << "kvadrant_odskok" << kvadrant_odskok;
    qWarning() << "kvadrant_kon" << kvadrant_kon;
    qWarning() << "alfa" << alfa;
    qWarning() << "alfa_krok" << alfa_k;
    qWarning() << "alfa_skok" << alfa_skok;
    qWarning() << "alfa_kon" << alfa_kon;
    qWarning() << "s_x" << s_x;
    qWarning() << "s_y" << s_y;
    qWarning() << "x_aktualni" << x_aktualni;
    qWarning() << "y_aktualni" << y_aktualni;
    qWarning() << "x_kon" << x_kon;
    qWarning() << "y_kon" << y_kon;
    qWarning() << "r " << r;
    qWarning() << "----------------------------------------------------";
*/
    for(int i = 0; i  < p; i++){
        alfa_prechodova = alfa_kon+(360 - alfa);

        //if(kvadrant_odskok ==4 && kvadrant_kon == 1){ // interpolace pri prechodu ze 4. kvadrantu do 1. kvadrantu
        if(kvadrant_kon < kvadrant_odskok){ // interpolace pri prechodu ze 4. kvadrantu do 1. kvadrantu

        //qWarning() << "alfa" << " " << alfa;
        if(alfa >= 360 && stopka == 0){
            alfa_skok = 0;
            stopka = 1;
        }
        //qWarning() << a << " " << b;
        if(alfa>= 0 && alfa< 90){
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y + b;
        kvadrant_vyp = 1;
        }
        if(alfa>= 90 && alfa< 180){
        if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y + b;
        kvadrant_vyp = 2;
        }
        if(alfa>= 180 && alfa< 270){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y - b;
        kvadrant_vyp = 3;
        }

        if(alfa >= 270 && alfa< 360){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y - b;
        kvadrant_vyp = 4;
        }
        if(alfa>= 360 && alfa< 450){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y + b;
        kvadrant_vyp = 5;
        }
        if(alfa>= 450 && alfa< 540){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y + b;
        kvadrant_vyp = 6;
        }
        if(alfa>= 540 && alfa< 630){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y - b;
        kvadrant_vyp = 7;
        }
        if(alfa >= 630 && alfa< 720){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y - b;
        kvadrant_vyp = 8;
        }
        prevod_na_m_kod(x_01 , x_02);
        x_aktualni = x_01;
        y_aktualni = x_02;

        qWarning() << x_01 << ";" << x_02 ;

        if(alfa_prechodova <= 0) break;

        }
        else{ // interploace pro ostatní kvadranty
            //qWarning() << "alfa" << " " << alfa;
            //qWarning() << "alfa skok" << alfa_skok;
            b = r * qSin(alfa_skok*PI/180);
            a = r * qCos(alfa_skok*PI/180);
            //qWarning() << a << " " << b;
            if(alfa>= 0 && alfa< 90){
                b = r * qSin(alfa_skok*PI/180);
                a = r * qCos(alfa_skok*PI/180);
                x_01 = s_x + a;
                x_02 = s_y + b;
                kvadrant_vyp = 1;
            }
            if(alfa>= 90 && alfa< 180){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qCos(alfa_skok*PI/180);
                a = r * qSin(alfa_skok*PI/180);
            x_01 = s_x - a;
            x_02 = s_y + b;
            kvadrant_vyp = 2;
            }
            if(alfa>= 180 && alfa< 270){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qSin(alfa_skok*PI/180);
                a = r * qCos(alfa_skok*PI/180);
            x_01 = s_x - a;
            x_02 = s_y - b;
            kvadrant_vyp = 3;
            }

            if(alfa >= 270 && alfa< 360){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qCos(alfa_skok*PI/180);
                a = r * qSin(alfa_skok*PI/180);
            x_01 = s_x + a;
            x_02 = s_y - b;
            kvadrant_vyp = 4;
            }
            prevod_na_m_kod(x_01 , x_02);
            x_aktualni = x_01;
            y_aktualni = x_02;

            qWarning() << x_01 << ";" << x_02 ;


            if(alfa >= alfa_kon)break;
        }


        alfa += alfa_k;
        alfa_skok += alfa_k;
    }
    //qWarning() << "kvadrant vypoctu" << kvadrant_vyp;
    //qWarning() << "=======================================================";
    stopka = 0;
}
/**
 * Metoda interpolující body pro funkci G02
 * @brief vlakno::interpolace_kruznice_g02
 * @param x_kon
 * @param y_kon
 * @param i
 * @param j
 */
void vlakno::interpolace_kruznice_g02(double x_kon, double y_kon, double i, double j){

    counter++;
   // qWarning() << counter;
    int stopka = 0;
    int kvadrant_odskok;
    int kvadrant_kon = 0;
    int kvadrant_vyp;
    double r, s_x, s_y, a, b, alfa,alfa_k, alfa_skok, alfa_kon_rad, alfa_kon, alfa_skok_rad,alfa_prechodova, x_01, x_02;
    double p;
    alfa = 0;
    alfa_k = 0;
    alfa_kon = 0;
    alfa_kon_rad = 0;
    alfa_skok = 0;
    alfa_skok_rad = 0;

    s_x = x_aktualni + i;
    s_y = y_aktualni + j;  // tady uz mam definovany stred kruznice
    /*ted budu vatvaret vektro S a koncovy bod tedy v = koncovy_bod - S =(x_kon - s_x; y_kon - s_y)
     *a to abych mohl vypocitat velikost r
     */
    //r = qSqrt((y_aktualni - s_x) * (x_aktualni - s_x) + (y_aktualni - s_y) * (y_aktualni - s_y));
    r = qSqrt((i*i)+(j*j));
    //qWarning() << "r:" << r;
    /*jelikoz mame prumer tak muzu urcit z kolika bodu se ta kruznice bude skladat (rozliseni) */
    p = r * z; //  p = mnozstvi bodu tvoricich  kruznici
    alfa = 360 / p; // určení velikosti uhlu v zavislosti na počtu vzorkovacích bodu
    alfa_k = alfa; // alfa_k ke velikost uhlu jednoho kroku
    //qWarning() << "p: "<< p << " alfa_k: " << alfa_k << "\n";

    if(y_aktualni >= s_y && x_aktualni >= s_x){   // vypocet odskokoveho pro 1. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((x_aktualni - s_x)/(y_aktualni-s_y));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = 90 - alfa_skok;
    kvadrant_odskok = 1;
    }
    if(y_aktualni > s_y && x_aktualni < s_x){   // vypocet odskokoveho pro 2. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((y_aktualni-s_y)/(s_x - x_aktualni));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa = 180 - alfa_skok;
    kvadrant_odskok = 2;
    }
    if(y_aktualni <= s_y && x_aktualni <= s_x){   // vypocet odskokoveho pro 3. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((s_x - x_aktualni)/(s_y - y_aktualni));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa =  270 - alfa_skok;
    kvadrant_odskok = 3;
    }
    if(y_aktualni < s_y && x_aktualni > s_x){   // vypocet odskokoveho pro 4. kvadrant_odskok
    //------------------------------vypocet odskokoveho uhlu------------------
    alfa_skok_rad = qAtan((s_y - y_aktualni)/(s_x - x_aktualni));
    alfa_skok =(alfa_skok_rad * 180) / PI;
    if(alfa_skok < 0) alfa_skok *= -1;
    alfa =  360 - alfa_skok;
    kvadrant_odskok = 4;
    }

    if(y_kon >= s_y && x_kon >= s_x){   // vypocet konecneho pro 1. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((x_kon - s_x)/(y_kon - s_y));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    alfa_kon = 90 - alfa_kon;
    if(alfa_kon < 0) alfa_kon *= -1;
    kvadrant_kon = 1;
    }
    if(y_kon > s_y && x_kon < s_x){   // vypocet konecneho pro 2. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((y_kon - s_y)/(s_x - x_kon));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon = 180 - alfa_kon;
    kvadrant_kon = 2;
    }
    if(y_kon <= s_y && x_kon <= s_x){   // vypocet konecneho pro 3. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((s_x - x_kon)/(s_y - y_kon));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon = 270 - alfa_kon;
    kvadrant_kon = 3;
    }
    if(y_kon < s_y && x_kon > s_x){   // vypocet konecneho pro 4. kvadrant_kon
    //------------------------------vypocet konecneho uhlu--------------------
    alfa_kon_rad = qAtan((s_y - y_kon)/(x_kon - s_x));
    alfa_kon = (alfa_kon_rad * 180) / PI;
    if(alfa_kon < 0) alfa_kon *= -1;
    alfa_kon = 360 - alfa_kon;
    kvadrant_kon = 4;
    }
/*
    qWarning() << "G02";
    qWarning() << "----------------------------------------------------";
    qWarning() << "x_aktual" << x_aktualni;
    qWarning() << "y_aktual" << y_aktualni;
    qWarning() << "kvadrant_odskok" << kvadrant_odskok;
    qWarning() << "kvadrant_kon" << kvadrant_kon;
    qWarning() << "alfa" << alfa;
    qWarning() << "alfa_krok" << alfa_k;
    qWarning() << "alfa_skok" << alfa_skok;
    qWarning() << "alfa_kon" << alfa_kon;
    qWarning() << "s_x" << s_x;
    qWarning() << "s_y" << s_y;
    qWarning() << "x_aktualni" << x_aktualni;
    qWarning() << "y_aktualni" << y_aktualni;
    qWarning() << "x_kon" << x_kon;
    qWarning() << "y_kon" << y_kon;
    qWarning() << "r " << r;
    qWarning() << "----------------------------------------------------";
*/
    for(int i = 0; i  < p; i++){
        alfa_prechodova = (360 + alfa) - alfa_kon;

        //if(kvadrant_odskok ==4 && kvadrant_kon == 1){ // interpolace pri prechodu ze 4. kvadrantu do 1. kvadrantu
        if(kvadrant_kon > kvadrant_odskok){ // interpolace pri prechodu ze 4. kvadrantu do 1. kvadrantu

        //qWarning() << "alfa" << " " << alfa;
        //qWarning() << "alfa_skok" << alfa_skok;
        if(alfa > 360 && stopka == 0){
            alfa_skok = 0;
            stopka = 1;
        }
        //qWarning() << a << " " << b;
        if(alfa> 0 && alfa< 90){
        if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y + b;
        kvadrant_vyp = 1;
        }
        if(alfa == 90){
            x_01 ==s_x;
            x_02 = s_y + r;
        }
        if(alfa> 90 && alfa< 180){
        if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y + b;
        kvadrant_vyp = 2;
        }
        if(alfa == 180){
            x_01 = s_x + r;
            x_02 = s_y;
        }
        if(alfa> 180 && alfa< 270){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y - b;
        kvadrant_vyp = 3;
        }
        if(alfa == 270){
            x_01 = s_x;
            x_02 = s_y + r;
        }
        if(alfa > 270 && alfa< 360){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y - b;
        kvadrant_vyp = 4;
        }
        if(alfa == 0){
            x_01 = s_x + r;
            x_02 = s_y;
        }
        if(alfa < 0 && alfa> -90){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y - b;
        kvadrant_vyp = 5;
        }
        if(alfa == -90){
            x_01 = s_x;
            x_02 = s_y + r;
        }
        if(alfa < -90 && alfa> -180){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y - b;
        kvadrant_vyp = 6;
        }
        if(alfa == -180){
            x_01 = s_x + r;
            x_02 = s_y;
        }
        if(alfa < -180 && alfa> -270){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qSin(alfa_skok*PI/180); // po y ose
        a = r * qCos(alfa_skok*PI/180); // po x ose
        x_01 = s_x - a;
        x_02 = s_y + b;
        kvadrant_vyp = 7;
        }
        if(alfa == -270){
            x_01 = s_x;
            x_02 = s_y + r;
        }
        if(alfa< -270 && alfa> -360){
            if(alfa_skok > 90) alfa_skok -= 90;
        b = r * qCos(alfa_skok*PI/180); // po y ose
        a = r * qSin(alfa_skok*PI/180); // po x ose
        x_01 = s_x + a;
        x_02 = s_y + b;
        kvadrant_vyp = 8;
        }
        if(alfa == -360){
            x_01 = s_x + r;
            x_02 = s_y;
        }
        prevod_na_m_kod(x_01 , x_02);
        x_aktualni = x_01;
        y_aktualni = x_02;
        //qWarning() << "kvadrant vypoctu" << kvadrant_vyp;
        qWarning() << x_01 << ";" << x_02 ;

        if(alfa_prechodova <= 0) break;

        }
        else{ // interploace pro ostatní kvadranty
            //qWarning() << "alfa" << " " << alfa;
            //qWarning() << "alfa skok" << alfa_skok;
            b = r * qSin(alfa_skok*PI/180);
            a = r * qCos(alfa_skok*PI/180);
            //qWarning() << a << " " << b;
            if(alfa>= 0 && alfa< 90){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qCos(alfa_skok*PI/180);
                a = r * qSin(alfa_skok*PI/180);
                x_01 = s_x + a;
                x_02 = s_y + b;
                kvadrant_vyp = 1;
            }
            if(alfa == 90){
                x_01 = s_x;
                x_02 = s_y + r;
            }
            if(alfa> 90 && alfa< 180){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qSin(alfa_skok*PI/180);
                a = r * qCos(alfa_skok*PI/180);
            x_01 = s_x - a;
            x_02 = s_y + b;
            kvadrant_vyp = 2;
            }
            if(alfa == 180){
                x_01 = s_x + r;
                x_02 = s_y;
            }
            if(alfa > 180 && alfa < 270){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qCos(alfa_skok*PI/180);
                a = r * qSin(alfa_skok*PI/180);
            x_01 = s_x - a;
            x_02 = s_y - b;
            kvadrant_vyp = 3;
            }
            if(alfa == 270){
                x_01 = s_x;
                x_02 = s_y - r;
            }
            if(alfa > 270 && alfa< 360){
                if(alfa_skok > 90) alfa_skok -= 90;
                b = r * qSin(alfa_skok*PI/180);
                a = r * qCos(alfa_skok*PI/180);
            x_01 = s_x + a;
            x_02 = s_y - b;
            kvadrant_vyp = 4;
            }
            if(alfa == 360){
                x_01 = s_x + r;
                x_02 = s_y;
            }

            prevod_na_m_kod(x_01 , x_02);
            x_aktualni = x_01;
            y_aktualni = x_02;

            qWarning() << x_01 << ";" << x_02 ;


            if(alfa <= alfa_kon)break;

        }
        alfa -= alfa_k;
        alfa_skok += alfa_k;


    }
    //qWarning() << "=======================================================";
    stopka = 0;
}
/**
 * Odpoji (pokud jiz nejsou odpojeny) vystupni a vystupni soubor a seriove pripojeni.
 * @brief vlakno::odpojit
 */
void vlakno::odpojit(){
    x.clear();
    y.clear();
    if(serial.isOpen()) serial.close();
    if(x_y_z.isOpen()) x_y_z.close();
    if(g_code.isOpen()) g_code.close();
    nahled->close();
    //delete nahled;
}
