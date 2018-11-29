#include "rrdialog.h"
#include "ui_rrdialog.h"

RRDialog::RRDialog(QString username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RRDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    ui->label_2->setText(username);
    timer = nullptr;
}

RRDialog::~RRDialog()
{
    delete ui;
}

void RRDialog::on_TimerClosed(){
    //timer->stop();
    close();
}

void RRDialog::on_pushButton_clicked()
{
    emit yes();
    close();
}

void RRDialog::on_pushButton_2_clicked()
{
    close();
}
