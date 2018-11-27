#include "alertdialog.h"
#include "ui_alertdialog.h"

AlertDialog::AlertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
}

AlertDialog::~AlertDialog()
{
    delete ui;
}

void AlertDialog::setText_0(QString msg){
    ui->label_0->setText(msg);
}
void AlertDialog::setText_1(QString msg){
    ui->label_1->setText(msg);
}

