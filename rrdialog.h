#ifndef RRDIALOG_H
#define RRDIALOG_H

#include <QDialog>

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

private:
    Ui::RRDialog *ui;
};

#endif // RRDIALOG_H
