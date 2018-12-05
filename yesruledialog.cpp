#include "yesruledialog.h"
#include "ui_yesruledialog.h"

YesruleDialog::YesruleDialog(int userid, int versionid, QString code, QString no, QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::yesruleDialog)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    this->userid = userid;
    this->versionid = versionid;
    this->code = code;
    this->no = no;
    ui->label_code->setText(code);
    ui->label_no->setText(no);
}

YesruleDialog::~YesruleDialog()
{
    delete ui;
}

void YesruleDialog::on_Button_yes_clicked()
{
    QString text = ui->textEdit->toPlainText();
    if(text.isEmpty()){
        ui->label_hint->setText(QString::fromLocal8Bit("确认合规时备注不能为空"));
    }
    else {
        QNetworkRequest request;
        QString url_str;
        url_str.append("http://120.78.190.36:9601/yesrule?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&versionid=");
        url_str.append(QString::number(versionid));
        url_str.append("&invoicecode=");
        url_str.append(code);
        url_str.append("&invoicenumber=");
        url_str.append(no);
        url_str.append("&remarks=");
        url_str.append(text);
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        //连接请求结束信号
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Yes(QNetworkReply*)));
    }

}

void YesruleDialog::on_Button_no_clicked()
{
    QNetworkRequest request;
    QString url_str;
    url_str.append("http://120.78.190.36:9601/norule?userid=");
    url_str.append(QString::number(userid));
    url_str.append("&versionid=");
    url_str.append(QString::number(versionid));
    request.setUrl(QUrl(url_str));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);
    //连接请求结束信号
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_No(QNetworkReply*)));
}

void YesruleDialog::on_textEdit_textChanged()
{
    QString strText = ui->textEdit->toPlainText();
    int length = strText.count();
    int maxLength = 40;
    if(length > maxLength)
    {
        int position = ui->textEdit->textCursor().position();
        strText.remove(position-(length-maxLength),length-maxLength);
        ui->textEdit->setText(strText);
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.setPosition(position-(length-maxLength));
        ui->textEdit->setTextCursor(cursor);
        ui->label_hint->setText(QString::fromLocal8Bit("备注长度达到极限"));
    }
    else{
        ui->label_hint->setText(QString::fromLocal8Bit(""));
    }
}

void YesruleDialog::on_Yes(QNetworkReply* reply){
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
                        ui->label_hint->setText(QString::fromLocal8Bit("已确认合规"));
                        ui->Button_no->setEnabled(false);
                        ui->Button_yes->setEnabled(false);
                        emit YesRule(userid, versionid, code, no);
                        close();
                        return;
                    }
                }
            }
        }
    }
}

void YesruleDialog::on_No(QNetworkReply* reply){
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
                        emit NoRule(userid, versionid, code, no);
                        ui->label_hint->setText(QString::fromLocal8Bit("已确认不合规"));
                        ui->Button_no->setEnabled(false);
                        ui->Button_yes->setEnabled(false);
                        timer = new QTimer(this);
                        connect(timer, SIGNAL(timeout()), this, SLOT(on_TimerClose()));
                        timer->start(400);
                        return;
                    }
                }
            }
        }
    }
}

void YesruleDialog::on_TimerClose(){
    timer->stop();
    delete timer;
    close();
}
