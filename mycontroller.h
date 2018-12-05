#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "run.h"
#include "qocr.h"
#include <QString>
#include <QThread>
#include <QSize>
#include <QPixmap>



class MyController: public QThread
{
    Q_OBJECT
public:
    MyController();
    virtual ~MyController();
    void run();
    void stop();
    void start();
    void setImageSize(QSize);
private:
    QOcr *qocr;
    QMutex mutex;
    QSize imgsize;

signals:
    void Message(QString sum1,QString sum2, QPixmap image,bool flag);

private slots:
    void onMessage(QString sum1,QString sum2, QPixmap image,bool flag);
};

#endif // MYCONTROLLER_H
