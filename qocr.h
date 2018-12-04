#ifndef QOCR_H
#define QOCR_H

#include "run.h"
#include <QObject>

class QOcr: public QObject, public OCRecognizer
{
    Q_OBJECT
public:
    QOcr();
    virtual ~QOcr();
    void message(string sum1,string sum2, Mat image,bool flag);
signals:
    void newMessage(QString sum1,QString sum2, Mat image,bool flag);
};

#endif // QOCR_H
