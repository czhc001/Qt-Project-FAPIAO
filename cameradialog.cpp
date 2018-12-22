#include "cameradialog.h"
#include "ui_cameradialog.h"

cameraDialog::cameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cameraDialog)
{
    ui->setupUi(this);
}

cameraDialog::~cameraDialog()
{
    delete ui;
}
