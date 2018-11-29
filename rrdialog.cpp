#include "rrdialog.h"
#include "ui_rrdialog.h"

RRDialog::RRDialog(QString username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RRDialog)
{
    ui->setupUi(this);
    ui->label_2->setText(username);
}

RRDialog::~RRDialog()
{
    delete ui;
}

void RRDialog::on_pushButton_clicked()
{
    emit yes();
    close();
}
