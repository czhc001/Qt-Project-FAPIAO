#include "severaddress.h"
#include <QCoreApplication>
#include <QFile>
#include <QByteArray>

QString SeverAddress::address = "";

SeverAddress::SeverAddress()
{

}

bool SeverAddress::loadAddress(){
    QString path = QCoreApplication::applicationDirPath();
    QString filename = "/server_info.txt";
    QFile file(path + filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    if(!file.atEnd()){
        QByteArray line = file.readLine();
        address = QString(line);
        address = address.remove(QRegExp("\\s"));
        return true;
    }
    else{
        return false;
    }
    return false;
}
