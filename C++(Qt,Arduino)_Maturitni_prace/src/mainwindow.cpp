#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QtCore>
#include <math.h>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include "qcustomplot.h"
#include <QtSerialPort>
/*! \file */
QString soubor;
vlakno* vlakno_;
QSerialPort serial_manual;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    aktualizovat_com();


}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Start tisku, tvorba vlákna
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    serial_manual.close();
    qDebug() << "port" << ui->com->currentText();
    qDebug() << "soubor" << soubor;
    vlakno_ = new vlakno;
    connect(vlakno_,SIGNAL(postup(int)),this,SLOT(na_pokrok(int)));
    connect(vlakno_,SIGNAL(stav(QString)),this,SLOT(na_zmenu_stavu(QString)));
    vlakno_->port = ui->com->currentText();
    vlakno_->filename = soubor;
    ui->pozastavit->setText("Pozastavit");
    if(!vlakno_->isRunning()) vlakno_->start();


}
/**
 * Aktualizace stavového labelu
 * @brief MainWindow::na_zmenu_stavu
 * @param stav_qstr
 */
void MainWindow::na_zmenu_stavu(QString stav_qstr){
    ui->status->setText(stav_qstr);
    if(stav_qstr =="Tisk dokončen")pripoj_manual();
}
/**
 * Určení hodnoty progressbaru
 * @brief MainWindow::na_pokrok
 * @param procento
 */
void MainWindow::na_pokrok(int procento){
    ui->progressbar->setValue(procento);
}
/**
 * Vybrat soubor s g_codem
 * @brief MainWindow::on_actionVybrat_soubor_triggered
 */
void MainWindow::on_actionVybrat_soubor_triggered()
{
    soubor = QFileDialog::getOpenFileName(this);
    qWarning() << "soubor" << soubor;
}
/**
 * Aktualizace připojených zařízení
 * @brief MainWindow::aktualizovat_com
 */
void MainWindow::aktualizovat_com(){
    ui->com->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
           QString coms = info.portName();
           qDebug() << coms;
           ui->com->addItem(coms);
    }

}
/**
 * Aktualizace připojených zařízení
 * @brief MainWindow::on_aktualizovat_com_clicked
 */
void MainWindow::on_aktualizovat_com_clicked()
{
    aktualizovat_com();
}
/**
 * Pozastavení vlákna změnou proměné pasue ve vlákně
 * @brief MainWindow::on_pozastavit_clicked
 */
void MainWindow::on_pozastavit_clicked()
{
    if(vlakno_->isRunning()){
        if(ui->pozastavit->text() == "Pozastavit"){
            vlakno_->pause = true;
            ui->pozastavit->setText("Obnovit");
            ui->status->setText("Pozastaveno");
            qWarning() << "pozastaveno";
        }else{
            vlakno_->pause = false;
            ui->pozastavit->setText("Pozastavit");
            ui->status->setText("Probíhá tisk");
            qWarning() << "pozastaveno";
        }
    }else{
        ui->status->setText("Neprobíhá tisk");
    }

}
/**
 * rusi tisk, ukončením vlákna
 * @brief MainWindow::on_zrusit_clicked
 */
void MainWindow::on_zrusit_clicked()
{
    if(vlakno_->isRunning()){
       vlakno_->odpojit();
       vlakno_->terminate();
       if(!serial_manual.isOpen()) pripoj_manual();
       ui->status->setText("Tisk zrušen");
       ui->progressbar->setValue(0);
       delete vlakno_;
    }else{
        ui->status->setText("Neprobíhá tisk");
    }

}
/**
 * Posila int data na seriovy port, pouzito pro manualni ovladani ploteru.
 * @brief MainWindow::poslat_int
 * @param zasilka_int
 * @return
 */
int MainWindow::poslat_int(int zasilka_int){
    QString data_qstr = QString::number(zasilka_int);
    const char *zasilka = data_qstr.toStdString().c_str();
    QByteArray responseData;


    serial_manual.write(zasilka);
    int waitTimeout = 5000;
    if (serial_manual.waitForBytesWritten(waitTimeout)) {
        // read response
        if (serial_manual.waitForReadyRead(waitTimeout)){
            responseData = serial_manual.read(1);
            if(responseData == "c"){
                //qWarning() << "odpovida";
                return 1;
            }
        } else  {

            qWarning() <<"neodpovida";
            serial_manual.write(zasilka);
            if (serial_manual.waitForBytesWritten(waitTimeout)) {
                // read response
                if (serial_manual.waitForReadyRead(waitTimeout)){
                    responseData = serial_manual.read(1);
                    if(responseData == "c"){
                        //qWarning() << "odpovida";
                        return 1;
                    }
                }
            }

        }
    } else {
       ui->status->setText("Problém s připojením");
    }
}
/**
 * Posle prikaz ploteru aby dal pero nahoru.
 * @brief MainWindow::on_dolu_nahoru_clicked
 */
void MainWindow::on_dolu_nahoru_clicked()
{
    poslat_int(600);
    poslat_int(1);

}
/**
 * Posle prikaz ploteru aby popojel po y vpred.
 * @brief MainWindow::on_y50_clicked
 */
void MainWindow::on_y50_clicked()
{
    poslat_int(602);
    poslat_int(500);
}
/**
 * Posle prikaz ploteru aby popojel po y zpet.
 * @brief MainWindow::on_ym50_clicked
 */
void MainWindow::on_ym50_clicked()
{
    poslat_int(602);
    poslat_int(-500);
}
/**
 * Posle prikaz ploteru aby popojel po x zpet.
 * @brief MainWindow::on_xm50_clicked
 */
void MainWindow::on_xm50_clicked()
{
    poslat_int(601);
    poslat_int(-500);
}
/**
 * Posle prikaz ploteru aby popojel po x vpred.
 * @brief MainWindow::on_x50_clicked
 */
void MainWindow::on_x50_clicked()
{
    poslat_int(601);
    poslat_int(500);
}
/**
 * Pokusi se pripojit k seriovemu portu kvuli manualnimu ovladani. Při zmene volby seriovemo portu.
 * @brief MainWindow::on_com_currentTextChanged
 * @param arg1
 */
void MainWindow::on_com_currentTextChanged(const QString &arg1)
{
    pripoj_manual();
}

/**
 * Metoda pripojeni k seriovemu portu, pro manualni ovladani
 * @brief MainWindow::pripoj_manual
 */
void MainWindow::pripoj_manual(){
    serial_manual.close();
    serial_manual.setPortName(ui->com->currentText());
    serial_manual.setBaudRate(QSerialPort::Baud9600);
    serial_manual.setDataBits(QSerialPort::Data8);
    serial_manual.setParity(QSerialPort::NoParity);
    serial_manual.setStopBits(QSerialPort::OneStop);
    serial_manual.setFlowControl(QSerialPort::NoFlowControl);
    if(!serial_manual.open(QIODevice::ReadWrite)){

        qWarning() << serial_manual.errorString();
        ui->status->setText("nelze se připojit k usb");
    }
}

/**
 * Posle prikaz ploteru aby dal pero dolu.
 * @brief MainWindow::on_dolu_clicked
 */
void MainWindow::on_dolu_clicked()
{
    poslat_int(600);
    poslat_int(0);
}

