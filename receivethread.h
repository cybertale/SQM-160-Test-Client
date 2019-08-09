#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QMainWindow>
#include <QThread>
#include <QSerialPort>
#include <QWaitCondition>

#include "crc14.h"

class ReceiveThread : public QThread
{
    Q_OBJECT

public:
    explicit ReceiveThread(QObject *parent = nullptr, QSerialPort *port = nullptr);

signals:
    void receiveData(const QByteArray &array);
    void errorMessage(const QString &msg);

private:
    void run() override;
    void parseBuffer();

    QSerialPort *port;
    QByteArray buffer;
    CRC14 crc;
};

#endif // RECEIVETHREAD_H
