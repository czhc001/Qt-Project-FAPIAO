#include "modifyuserdialog.h"
#include "ui_modifyuserdialog.h"

ModifyUserDialog::ModifyUserDialog(int userid_manager, int userid, QString username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyUserDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    ui->lineEdit_username->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_passwords->setAttribute(Qt::WA_InputMethodEnabled, false);
    this->userid_manager = userid_manager;
    this->userid = userid;
    this->ui->lineEdit_username->setText(username);
    this->ui->lineEdit_username->setEnabled(false);
    timer = nullptr;
}

ModifyUserDialog::~ModifyUserDialog()
{
    delete ui;
}

void ModifyUserDialog::on_pushButton_clicked()
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
        ui->label_hint->setText(QString::fromLocal8Bit("修改中"));
        QNetworkRequest request;
        QString url_str;
        url_str.append("http://120.78.190.36:9601/changeuser?userid=");
        url_str.append(QString::number(userid_manager));
        url_str.append("&bechangedid=");
        url_str.append(QString::number(userid));
        url_str.append("&username=");
        url_str.append(username);
        url_str.append("&password=");
        url_str.append(passwords);
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        //连接请求结束信号
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(modifyUserResult(QNetworkReply*)));
        //连接响应时返回数据信号
        //connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    }
}

void ModifyUserDialog::modifyUserResult(QNetworkReply* reply){
    QByteArray data = reply->readAll();
    QString result = QString::fromStdString(data.toStdString()).toUtf8();
    qDebug() << "!!!!" << result;
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
                            ui->label_hint->setText(QString::fromLocal8Bit("修改成功"));
                            emit modifiedUser(username);
                            timer = new QTimer();
                            connect(timer, SIGNAL(timeout()), this, SLOT(on_TimerClose()));
                            timer->start(600);
                        }
                        else{
                            ui->label_hint->setText(QString::fromLocal8Bit("修改失败"));
                        }
                    }
                }
            }
        }
    }
    delete reply;
}

void ModifyUserDialog::on_TimerClose(){
    close();
}
