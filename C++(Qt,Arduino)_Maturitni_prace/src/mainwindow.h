#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vlakno.h"
/*! \file */
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    vlakno *vlakno_;
private slots:
    void on_pushButton_clicked();

    void on_actionVybrat_soubor_triggered();

    void aktualizovat_com();

    void on_aktualizovat_com_clicked();

    void na_zmenu_stavu(QString stav_qstr);

    void na_pokrok(int procento);



    void on_pozastavit_clicked();

    void on_zrusit_clicked();

    int poslat_int(int zasilka_int);

    void on_dolu_nahoru_clicked();

    void on_y50_clicked();

    void on_ym50_clicked();

    void on_xm50_clicked();

    void on_x50_clicked();

    void pripoj_manual();

    void on_com_currentTextChanged(const QString &arg1);

    void on_dolu_clicked();


signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
