#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "mainwindow.h"
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

private:
    Ui::LoginWindow *ui;
    AlertDialog *alertDialog;
    QString username;
    void initializa_UI();

};

#endif // LOGINWINDOW_H
