#include "qocr.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>

QOcr::QOcr()
{
    qRegisterMetaType<Mat>("Mat");
    QString path = QCoreApplication::applicationDirPath();
    QString model0 = "/models/lenet.prototxt";
    QString model1 = "/models/lenet_iter_10000.caffemodel";
    QString modelpath0 = path + model0;
    QString modelpath1 = path + model1;
    qDebug() << "P0:: " << modelpath0;
    qDebug() << "P1:: " << modelpath1;
    QByteArray ba0 = modelpath0.toLocal8Bit();
    QByteArray ba1 = modelpath1.toLocal8Bit();
    char * p0 = ba0.data();
    char * p1 = ba1.data();
    qDebug() << "std0" << p0;
    qDebug() << "std1" << p1;
    set_modelpath0(p0);
    set_modelpath1(p1);
}

QOcr::~QOcr()
{
    qDebug() << "delete QOCR";
}

void QOcr::message(string sum1,string sum2, Mat image,bool flag){
    QString m1 = QString::fromStdString(sum1);
    QString m2 = QString::fromStdString(sum2);
    //qDebug() << QString("!!!") << m1 << QString("!!!") <<m2;
    //qDebug() << "QOcr emited";
    emit newMessage(m1, m2, image, flag);
}
