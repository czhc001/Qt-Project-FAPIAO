#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QString title, QString content, QWidget *parent = 0);
    ~ConfirmDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ConfirmDialog *ui;

signals:
    void confirm();
};

#endif // CONFIRMDIALOG_H
