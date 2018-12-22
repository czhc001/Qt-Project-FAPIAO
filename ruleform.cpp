#include "ruleform.h"
#include "ui_ruleform.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <severaddress.h>

RuleForm::RuleForm(int userid, int permissionid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleForm)
{
    ui->setupUi(this);
    QRegExp wx_count("[0-9]{8}");
    QRegExpValidator *latitude = new QRegExpValidator(wx_count, this);
    ui->lineEdit->setValidator(latitude);
    this->userid = userid;
    this->permissionid = permissionid;
    on_pushButton_2_clicked();
}

RuleForm::~RuleForm()
{
    delete ui;
}

void RuleForm::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        ui->label_hint->setText(QString::fromLocal8Bit("请补全信息"));
        return;
    }
    QNetworkRequest request;
    QString url_str;
    url_str.append("http://" + SeverAddress::address + "/setrule?userid=");
    url_str.append(QString::number(userid));
    url_str.append("&permissionid=");
    url_str.append(QString::number(permissionid));
    url_str.append("&invoicenumber_range=");
    url_str.append(QString::number((local_n = ui->lineEdit->text().toInt())));
    request.setUrl(QUrl(url_str));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Reply(QNetworkReply*)));
}

void RuleForm::on_Reply(QNetworkReply* reply){
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
                    if(dataObj.isBool()){
                        bool r = dataObj.toBool();
                        if(r){
                            ui->label_hint->setText(QString::fromLocal8Bit("设置成功"));
                            ui->label_d->setText(QString::number(local_n));
                        }
                        else{
                            ui->label_hint->setText(QString::fromLocal8Bit("设置失败"));
                        }
                    }
                }
            }
        }
    }
}

void RuleForm::on_pushButton_2_clicked()
{
    QNetworkRequest request;
    QString url_str;
    url_str.append("http://" + SeverAddress::address + "/setrule?userid=");
    url_str.append(QString::number(userid));
    url_str.append("&permissionid=");
    url_str.append(QString::number(permissionid));
    url_str.append("&invoicenumber_range=");
    url_str.append(QString::number(-1));
    request.setUrl(QUrl(url_str));
    qDebug() << url_str;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Reply2(QNetworkReply*)));
}

void RuleForm::on_Reply2(QNetworkReply* reply){
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
                    if(dataObj.isDouble()){
                        int nn = dataObj.toInt();
                        ui->label_d->setText(QString::number(nn));
                        ui->label_hint2->setText("");
                    }
                    else{
                        ui->label_hint2->setText(QString::fromLocal8Bit("查询失败"));
                    }
                }
            }
        }
    }
}
