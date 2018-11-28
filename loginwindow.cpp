#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString passwords = ui->lineEdit_passwords->text();
    if(username.isEmpty())
    {
        ui->label_hint->setText(QString::fromLocal8Bit("用户名不能为空"));
    }
    else if(passwords.isEmpty())
    {
        ui->label_hint->setText(QString::fromLocal8Bit("密码不能为空"));
    }
    else{
        ui->label_hint->setText("login...");
        this->username = username;
        QNetworkRequest request;
        QString url_str;
        url_str.append("http://120.78.190.36:9601/login?username=");
        url_str.append(username);
        url_str.append("&password=");
        url_str.append(passwords);
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        //连接请求结束信号
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
        //连接响应时返回数据信号
        connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    }
}

void LoginWindow::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QString result = QString::fromStdString(data.toStdString()).toUtf8();
    qDebug() << result;
    QJsonDocument document;
    QJsonParseError ParseError;
    document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);
    if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
        if(document.isObject()){
            QJsonObject Object = document.object();
            if(!Object.isEmpty()){
                if(Object.contains("data")){
                    QJsonValue dataObj = Object.value("data");
                    if(dataObj.isObject()){  //判断是否是对象
                        QJsonObject ObjectData0 = dataObj.toObject();
                        if(ObjectData0.contains("permissionid") && ObjectData0.contains("userid")){
                            ui->label_hint->setText(QString::fromLocal8Bit("欢迎"));
                            int permissionid = ObjectData0.value("permissionid").toInt();
                            int userid = ObjectData0.value("userid").toInt();
                            qDebug() << permissionid << " : " << userid;
                            MainWindow * mainwindow = new MainWindow(userid, permissionid, username);
                            mainwindow->show();
                            this->close();
                        }
                    }
                    else if(dataObj.isBool()){
                        ui->label_hint->setText(QString::fromLocal8Bit("用户名或密码错误"));
                    }
                }
            }
        }
    }
}

void LoginWindow::on_readyRead()
{
    //....
}
