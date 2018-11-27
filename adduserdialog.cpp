#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include <QDebug>

AddUserDialog::AddUserDialog(int userid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    this->userid = userid;
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void AddUserDialog::on_regButton_clicked()
{
    username = ui->lineEdit_username->text();
    passwords = ui->lineEdit_passwords->text();
    if(username.isEmpty()){
        ui->label_hint->setText(QString::fromLocal8Bit("用户名不能为空"));
    }
    else if(passwords.isEmpty()){
        ui->label_hint->setText(QString::fromLocal8Bit("密码不能为空"));
    }
    else{
        ui->label_hint->setText(QString::fromLocal8Bit("注册中"));
        QNetworkRequest request;
        QString url_str;
        url_str.append("http://120.78.190.36:9601/adduser?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&username=");
        url_str.append(username);
        url_str.append("&password=");
        url_str.append(passwords);
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        //连接请求结束信号
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addUserResult(QNetworkReply*)));
        //连接响应时返回数据信号
        //connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    }



}

void AddUserDialog::addUserResult(QNetworkReply* reply){
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
                    if(dataObj.isBool()){  //判断是否是对象
                        bool isSuccessful = dataObj.toBool();
                        if(isSuccessful){
                            ui->label_hint->setText(QString::fromLocal8Bit("用户 ").append(username).append(QString::fromLocal8Bit(" 注册成功")));
                            emit user_Added(username);
                        }
                        else{
                            ui->label_hint->setText(QString::fromLocal8Bit("注册失败"));
                        }
                    }
                }
            }
        }
    }
}
