#include "mycontroller.h"
#include <QDebug>

MyController::MyController()
{
    qocr = nullptr;
    qRegisterMetaType<QPixmap>("QPixmap&");
}

MyController::~MyController()
{
    if(qocr != nullptr){
        qocr->stop();
        this->wait();
        delete qocr;
    }
    qDebug() << "delete MyController";
}

void MyController::onMessage(QString sum1,QString sum2, QPixmap &image,bool flag){
    //qDebug() << "MyController emited";
    emit Message(sum1, sum2, image, flag);
}

void MyController::start(){
    QMutexLocker locker(&mutex);
    QThread::start();
}

void MyController::run(){
    if(this->qocr == nullptr){
        qocr = new QOcr();
        qocr->setImageSize(this->imgsize);
        connect(qocr, SIGNAL(newMessage(QString, QString, QPixmap &, bool)), this, SLOT(onMessage(QString, QString, QPixmap &, bool)));
        qocr->run();
    }
}

void MyController::stop(){
    QMutexLocker locker(&mutex);
    if(qocr != nullptr){
        qocr->stop();
        this->wait();
        delete qocr;
        qocr = nullptr;
    }
}

void MyController::setImageSize(QSize size){
    imgsize = size;
}
