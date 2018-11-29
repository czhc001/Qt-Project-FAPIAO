#ifndef RRDIALOG_H
#define RRDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class RRDialog;
}

class RRDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RRDialog(QString, QWidget *parent = nullptr);
    ~RRDialog();
signals:
    void yes();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_TimerClosed();

private:
    Ui::RRDialog *ui;
    QTimer *timer;
};

#endif // RRDIALOG_H
