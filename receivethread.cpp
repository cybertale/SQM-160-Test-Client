#include "receivethread.h"
#include "mainwindow.h"

#include <QMessageBox>

//#define CHECK_CRC

ReceiveThread::ReceiveThread(QObject *parent, QSerialPort *port):
    QThread(parent)
{
    this->port = port;
}

void ReceiveThread::run()
{
    while (port->isOpen()) {
        port->waitForReadyRead(10);

        if (port->bytesAvailable() != 0) {
            QByteArray bufferReceived = port->readAll();

            buffer.append(bufferReceived);
            parseBuffer();
            //emit this->receiveData(array);
        }
    }
}

void ReceiveThread::parseBuffer()
{
    uint16_t i = 0;

    while(buffer[i] != '!') {
        i++;
    }

    //Remove starting none '!' characters.
    if (i > 0) {
        buffer.remove(0, i);
    }

    int len = buffer[1] - 35;
    if (buffer.count() >= len + 2 + 2) {
        crc.calcCRC(buffer.mid(0, len + 2));
#ifdef CHECK_CRC
        if (crc.crc1() == buffer[len + 2] && crc.crc2() == buffer[len + 3]) {
            emit receiveData(buffer.mid(1, len + 1));
        } else {
            emit errorMessage("CRC Error.");
        }
#else
        emit receiveData(buffer.mid(1, len + 1));
#endif
        buffer.remove(0, len + 2 + 2);
    }
}
