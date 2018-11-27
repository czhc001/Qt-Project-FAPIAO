#include "mycontroller.h"
#include <QDebug>w

MyController::MyController()
{
}

void MyController::message(int n0, int n1){
    QString str0 = QString::number(n0);
    QString str1 = QString::number(n1);
    emit newMessage(str0, str1);
}

void MyController::run_1(){
    Invoice::run();
}
