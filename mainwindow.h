#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#include "receivethread.h"
#include "crc14.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonGetVersion_clicked();
    void on_buttonOpenSerial_clicked();
    void receiveData(const QByteArray &array);
    void errorMessage(const QString &msg);

    void on_buttonGetRate_clicked();

private:
    Ui::MainWindow *ui;
    ReceiveThread receiveThread;
    QSerialPort port;
    CRC14 crc;
};

#endif // MAINWINDOW_H
