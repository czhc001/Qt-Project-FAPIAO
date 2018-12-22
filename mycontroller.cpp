#include "mycontroller.h"
#include <QDebug>
#include <severaddress.h>

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

void MyController::onErrorMessage(int a){
    emit errorMessage(a);
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
        connect(qocr, SIGNAL(errorMessage(int)), this, SLOT(onErrorMessage(int)));
    }
    if(!qocr->isInitialized()){
        qDebug() << "OCR Init";
        qocr->init();
    }
    qocr->run();
}

void MyController::stop(){
    QMutexLocker locker(&mutex);
    if(qocr != nullptr){
        qocr->stop();
        this->wait();
        //delete qocr;
        //aqocr = nullptr;
    }
}

void MyController::setImageSize(QSize size){
    imgsize = size;
}
