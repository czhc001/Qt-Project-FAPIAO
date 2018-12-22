#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QString title, QString content, QString accept = QString::fromLocal8Bit("确定"), QString cancel = QString::fromLocal8Bit("取消"), QWidget *parent = 0);
    ~MessageDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MessageDialog *ui;

signals:
    void confirm();
    void cancel();
};

#endif // MESSAGEDIALOG_H
