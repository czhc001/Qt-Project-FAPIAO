#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "run.h"
#include "qocr.h"
#include <QString>
#include <QThread>




class MyController: public QThread
{
    Q_OBJECT
public:
    MyController();
    virtual ~MyController();
    void run();
private:
    QOcr *qocr;

signals:
    void Message(QString sum1,QString sum2, Mat image,bool flag);

private slots:
    void onMessage(QString sum1,QString sum2, Mat image,bool flag);
};

#endif // MYCONTROLLER_H
