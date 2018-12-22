#include "quitdialog.h"
#include "ui_quitdialog.h"

QuitDialog::QuitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuitDialog)
{
    ui->setupUi(this);
}

QuitDialog::~QuitDialog()
{
    delete ui;
}

void QuitDialog::on_pushButton_clicked()
{
    qApp->quit();
}

void QuitDialog::on_pushButton_2_clicked()
{
    close();
}
