#ifndef MODIFYUSERDIALOG_H
#define MODIFYUSERDIALOG_H

#include <QDialog>
#include <QtNetwork>

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

private:
    Ui::ModifyUserDialog *ui;
    QString username;
    QString passwords;
    int userid;
    int userid_manager;
signals:
    void modifiedUser(QString username);
};

#endif // MODIFYUSERDIALOG_H
