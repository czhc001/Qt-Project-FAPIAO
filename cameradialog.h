#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>

namespace Ui {
class cameraDialog;
}

class cameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cameraDialog(QWidget *parent = 0);
    ~cameraDialog();

private:
    Ui::cameraDialog *ui;
};

#endif // CAMERADIALOG_H
