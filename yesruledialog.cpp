#include "yesruledialog.h"
#include "ui_yesruledialog.h"
#include <QStandardItemModel>
#include <QList>
#include <severaddress.h>

YesruleDialog::YesruleDialog(int userid, int versionid, int current_count, QString current_code, QString current_no, QString current_manager,
                             QString current_department, QString current_date, QString current_serial, QString current_username,
                             QJsonArray hisInfoArray, QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::yesruleDialog)
{
    //setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    this->userid = userid;
    this->versionid = versionid;
    this->current_code = current_code;
    this->current_no = current_no;
    this->current_count = current_count;
    //ui->label_code->setText(code);
    //ui->label_no->setText(no);
    dataModel = new QStandardItemModel();
    dataModel->setColumnCount(2 + hisInfoArray.size());
    dataModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("项目"));
    dataModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("当前发票"));
    for(int i = 0; i < hisInfoArray.size(); ++i){
        dataModel->setHeaderData(2+i, Qt::Horizontal, QString::fromLocal8Bit("历史发票") + QString::number(i+1));
    }
    ui->tableView->setModel(dataModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 225);
    for(int i = 0; i < hisInfoArray.size(); ++i){
        ui->tableView->setColumnWidth(2+i, 225);
    }

    QList<QJsonObject> history_objs;
    for(int i = 0; i < hisInfoArray.size(); ++i){
        history_objs.append(hisInfoArray.at(i).toObject());
    }
    int len = hisInfoArray.size();
    addRow(QString::fromLocal8Bit("发票代码"), current_code, history_objs, len, "invoicecode");
    addRow(QString::fromLocal8Bit("发票号码"), current_no, history_objs, len, "invoicenumber");
    addRow(QString::fromLocal8Bit("登记日期"), current_date, history_objs, len, "date");
    addRow(QString::fromLocal8Bit("经办人"), current_manager, history_objs, len, "manager");
    addRow(QString::fromLocal8Bit("申请部门"), current_department, history_objs, len, "department");
    addRow(QString::fromLocal8Bit("编号"), current_serial, history_objs, len, "serialnumber");
    addRow(QString::fromLocal8Bit("张数"), QString::number(current_count), history_objs, len, "queuenumber");
    addRow(QString::fromLocal8Bit("操作账号"), current_username, history_objs, len, "username");
    addRow(QString::fromLocal8Bit("备注"), current_username, history_objs, len, "remarks");

    ui->tableView->setRowHeight(0, 48);
    ui->tableView->setRowHeight(1, 48);
    ui->tableView->setRowHeight(2, 48);
    ui->tableView->setRowHeight(3, 48);
    ui->tableView->setRowHeight(4, 48);
    ui->tableView->setRowHeight(5, 48);
    ui->tableView->setRowHeight(6, 48);
    ui->tableView->setRowHeight(7, 48);

    setTabOrder(ui->textEdit, ui->Button_no);
    setTabOrder(ui->Button_no, ui->Button_yes);
}

void YesruleDialog::addRow(QString name, QString current, QList<QJsonObject> infos, int len, QString key_name){
    QList<QStandardItem *> list;
    QStandardItem * item_name = new QStandardItem(name);
    QStandardItem * item_current = new QStandardItem(current);
    QStandardItem ** item_history = new QStandardItem * [len];
    for(int i = 0; i < len; ++i){
        QJsonValue value = infos.at(i).value(key_name);
        if(value.isString()){
            item_history[i] = new QStandardItem(value.toString());
        }
        else {
            item_history[i] = new QStandardItem();
            item_history[i]->setData(QVariant(value.toInt()),Qt::EditRole);
        }
        item_history[i]->setFont(QFont( "Microsoft YaHei", 11, QFont::Black ));
        item_history[i]->setTextAlignment(Qt::AlignCenter);
    }

    item_name->setTextAlignment(Qt::AlignCenter);
    item_current->setTextAlignment(Qt::AlignCenter);

    item_name->setFont(QFont( "Microsoft YaHei", 11, QFont::Black ));
    item_current->setFont(QFont( "Microsoft YaHei", 11, QFont::Black ));


    list.append(item_name);
    list.append(item_current);
    for(int i = 0; i < len; ++i){
        list.append(item_history[i]);
    }

    dataModel->appendRow(list);
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
        url_str.append("http://" + SeverAddress::address + "/yesrule?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&versionid=");
        url_str.append(QString::number(versionid));
        url_str.append("&invoicecode=");
        url_str.append(current_code);
        url_str.append("&invoicenumber=");
        url_str.append(current_no);
        url_str.append("&remarks=");
        url_str.append(text);
        url_str.append("&completednumber=");
        url_str.append(QString::number(current_count));
        request.setUrl(QUrl(url_str));
        qDebug() << url_str;
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
    url_str.append("http://" + SeverAddress::address + "/norule?userid=");
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
    qDebug() << "YESRULE  " << result;
    QJsonDocument document;
    QJsonParseError ParseError;
    document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);
    if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
        if(document.isObject()){
            QJsonObject Object = document.object();
            if(!Object.isEmpty()){
                if(Object.contains("data")){
                    QJsonValue dataObj = Object.value("data");
                    if(dataObj.isObject()){
                        QJsonObject obj = dataObj.toObject();
                        int flag = obj.value("flag").toInt();
                        if(1 == flag){
                            ui->label_hint->setText(QString::fromLocal8Bit("已确认合规"));
                            ui->Button_no->setEnabled(false);
                            ui->Button_yes->setEnabled(false);
                            emit YesRule(userid, versionid, current_code, current_no);
                            close();
                            return;
                        }
                        else if(2 == flag){
                            ui->label_hint->setText(QString::fromLocal8Bit("已确认合规，本版结束"));
                            ui->Button_no->setEnabled(false);
                            ui->Button_yes->setEnabled(false);
                            emit Complete(userid, versionid, current_code, current_no);
                            close();
                            return;
                        }
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
                        emit NoRule(userid, versionid, current_code, current_no);
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
