#ifndef VLAKNO_H
#define VLAKNO_H

#include <QThread>
#include <QFile>
/*! \file */
class vlakno : public QThread
{
    Q_OBJECT
public:
    vlakno(QObject *parent = 0);
    void run();
    int poslat_int(int data);
    void zapsat(int data);
    void prevod_na_m_kod(double x_kon , double y_kon );
    void interpolace_usecky( double x_kon, double y_kon);
    void interpolace_kruznice_g03(double x_kon, double y_kon, double i, double j);
    void interpolace_kruznice_g02(double x_kon, double y_kon, double i, double j);
    bool stop;
    bool pause = false;
    QString filename;
    QString port;
    void odpojit();

signals:
    void postup(int);
    void stav(QString);

public slots:
};

#endif // VLAKNO_H
