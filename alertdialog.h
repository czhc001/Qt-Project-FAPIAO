#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>

namespace Ui {
class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(QWidget *parent = nullptr);
    ~AlertDialog();
    void setText_0(QString);
    void setText_1(QString);
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::AlertDialog *ui;
    bool isClosed;

    QTimer *timer;
    int time_left;
signals:
    void closed();
private slots:
    void updateText();
};

#endif // ALERTDIALOG_H
