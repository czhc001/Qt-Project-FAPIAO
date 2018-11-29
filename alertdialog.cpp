#include "alertdialog.h"
#include "ui_alertdialog.h"
#include <QTimer>

AlertDialog::AlertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
    time_left = 3;
    timer->start(1000);
}

AlertDialog::~AlertDialog()
{
    delete ui;
    delete timer;
}

void AlertDialog::setText_0(QString msg){
    ui->label_0->setText(msg);
}
void AlertDialog::setText_1(QString msg){

}

void AlertDialog::updateText(){
    --time_left;
    QString str = "(";
    str.append(QString::number(time_left));
    str.append(")");
    ui->label->setText(str);
    if(time_left <=0 )
        this->close();
}

