#include "messagedialog.h"
#include "ui_messagedialog.h"
#include <severaddress.h>

MessageDialog::MessageDialog(QString title, QString content, QString accept, QString cancel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    ui->label->setText(content);
    ui->pushButton->setText(accept);
    ui->pushButton_2->setText(cancel);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::on_pushButton_clicked()
{
    emit confirm();
    close();
}

void MessageDialog::on_pushButton_2_clicked()
{
    emit cancel();
    close();
}
