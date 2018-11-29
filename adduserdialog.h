#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(int userid, QWidget *parent = nullptr);
    ~AddUserDialog();

signals:
    void user_Added(QString username_added);

private slots:
    void on_regButton_clicked();
    void addUserResult(QNetworkReply*);
    void on_TimerClose();

private:
    Ui::AddUserDialog *ui;
    int userid;
    QString username;
    QString passwords;
    QTimer * timer;
};

#endif // ADDUSERDIALOG_H
