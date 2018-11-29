#ifndef MODIFYUSERDIALOG_H
#define MODIFYUSERDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class ModifyUserDialog;
}

class ModifyUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyUserDialog(int userid_manager, int userid, QString username, QWidget *parent = nullptr);
    ~ModifyUserDialog();

private slots:
    void on_pushButton_clicked();
    void modifyUserResult(QNetworkReply*);
    void on_TimerClose();

private:
    Ui::ModifyUserDialog *ui;
    QString username;
    QString passwords;
    int userid;
    int userid_manager;
    QTimer *timer;
signals:
    void modifiedUser(QString username);
};

#endif // MODIFYUSERDIALOG_H
