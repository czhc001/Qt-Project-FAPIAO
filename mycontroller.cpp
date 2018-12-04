#include "mycontroller.h"
#include <QDebug>

MyController::MyController()
{
    qocr = new QOcr();
    connect(qocr, SIGNAL(newMessage(QString, QString, Mat, bool)), this, SLOT(onMessage(QString, QString, Mat, bool)));
}

MyController::~MyController()
{
    qocr->stop();
    this->wait();
    delete qocr;
    qDebug() << "delete MyController";
}

void MyController::onMessage(QString sum1,QString sum2, Mat image,bool flag){
    //qDebug() << "MyController emited";
    emit Message(sum1, sum2, image, flag);
}

void MyController::run(){
    this->qocr->run();
}
