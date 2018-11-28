#include "mycontroller.h"
#include <QDebug>w

MyController::MyController()
{
}

void MyController::message(bool s, int n0, int n1, long image){
    QString str0 = QString::number(n0);
    QString str1 = QString::number(n1);
    emit newMessage(s, str0, str1, image);
}

void MyController::run_1(){
    Invoice::run();
}
