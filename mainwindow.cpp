#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>
#include <cstdio>

#include "crc14.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    receiveThread(this, &port)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo info : ports)
        ui->comboPorts->addItem(info.portName());

    connect(&receiveThread, &ReceiveThread::receiveData, this, &MainWindow::receiveData);
    connect(&receiveThread, &ReceiveThread::errorMessage, this, &MainWindow::errorMessage);

    //crc.calcCRC(QByteArray::fromStdString("!0AMON_Ver_4.13"));
    crc.calcCRC(QByteArray::fromStdString("!*A_0.00_"));
    QString str;
    str.sprintf("%x, %d, %d", crc.crc, crc.crc1(), crc.crc2());
    QMessageBox::information(this, "test", str);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (port.isOpen())
        port.close();
}

void MainWindow::on_buttonGetVersion_clicked()
{
    //QByteArray buffer = QByteArray::fromStdString("!#@");
    QByteArray buffer = QByteArray::fromStdString("!IC_5.000_6.000_0.000_10.00_0.000_10.00_0");

    crc.calcCRC(buffer);
    buffer.append(crc.crc1());
    buffer.append(crc.crc2());
    port.write(buffer, buffer.length());
}

void MainWindow::on_buttonOpenSerial_clicked()
{
    if (ui->comboPorts->count() != 0) {
        if (port.isOpen() == false) {

            port.setPortName(ui->comboPorts->currentText());
            bool ret = port.open(QIODevice::ReadWrite);
            if (ret == false) {
                QMessageBox::warning(this, "Error", "Port open failed.");
            } else {
                receiveThread.start();
                ui->buttonOpenSerial->setText("Close");
            }
        } else {
            receiveThread.exit();
            port.close();
            ui->buttonOpenSerial->setText("Open");
        }
    }
}

void MainWindow::receiveData(const QByteArray &array)
{
    ui->boxReceive->appendPlainText(QString::fromUtf8(array));
}

void MainWindow::errorMessage(const QString &msg)
{
    QMessageBox::warning(this, "Error", msg);
}


void MainWindow::on_buttonGetRate_clicked()
{
}
