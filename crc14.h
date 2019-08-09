#ifndef CRC14_H
#define CRC14_H

#include <QByteArray>

class CRC14
{
public:
    CRC14(void) { crc = 0x0; }

public:
    short crc;
public:
    short calcCRC(QByteArray str)
    {
        int length = (str != nullptr) ? 1 + str[1] - 34 : 0;
        if (length > 0) {
            crc = (short) 0x3fff;
            for (int jx = 1; jx <= length; jx++) {
                crc = (short)(crc ^ (short)str[jx] );
                for (int ix = 0; ix < 8; ix++) {
                    short tmpCRC = crc;
                    crc = (short) (crc >> 1);
                    if ((tmpCRC & 0x1) == 1) {
                        crc = (short) (crc ^ 0x2001);
                    }
                }
                crc = (short) (crc & 0x3fff);
            }
        }
        return crc;
    }

    char crc2() {
        return (unsigned char)((crc >> 7) & 0x7f) + 34;
    }

    char crc1() {
        return (unsigned char)(crc & 0x7f) + 34;
    }
};

#endif // CRC14_H
