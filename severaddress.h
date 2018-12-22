#ifndef SEVERADDRESS_H
#define SEVERADDRESS_H
#include <QString>

class SeverAddress
{
public:
    SeverAddress();
    static bool loadAddress();
    static QString address;
};

#endif // SEVERADDRESS_H
