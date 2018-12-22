#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "mainwindow_.h"
#include "alertdialog.h"
#include "managementwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();
    void replyFinished(QNetworkReply* reply);
    void on_readyRead();

    void on_lineEdit_passwords_returnPressed();

    void on_lineEdit_username_returnPressed();

private:
    Ui::LoginWindow *ui;
    AlertDialog *alertDialog;
    QString username;
    void initializa_UI();
    bool suc;
    QMutex mutex;

};

#endif // LOGINWINDOW_H
