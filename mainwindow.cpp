#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "managementwindow.h"
#include "yesruledialog.h"
#include <QFileDialog>
#include "alertdialog.h"

MainWindow::MainWindow(int userid, int permissionid, QString username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->userid = userid;
    this->username = username;
    this->permissionid = permissionid;
    ui->setupUi(this);

    if(permissionid != 1)
        ui->Button_manage->setEnabled(false);
    QString m_sProjectPath = tr("D:\\images");
    QDir dir(m_sProjectPath);
    dir.setFilter(QDir::Files);
    QFileInfoList fileList = dir.entryInfoList();
    int fileCount = fileList.count();
    for(int i=0;i<fileCount;++i){
        QString filepath = fileList[i].absoluteFilePath();
        qDebug() << filepath;
        QImage *img = new QImage(filepath);
        image_list.append(img);
    }


    timer = new QTimer(this);
    ii = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
    timer->start(50);
    control = new MyController();
    connect(control, SIGNAL(newMessage(bool, QString, QString, long)), this, SLOT(on_New_Message(bool, QString, QString, long)));
    isRunning = false;
    started = false;
    firstStarted = true;
    checking_current = false;
    unstablePassed = true;
    ready_for_current = true;
    initializa_UI();
    ui->lineEdit_coderesult->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_noresult->setAttribute(Qt::WA_InputMethodEnabled, false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializa_UI(){
    QPalette palette;
//    palette.setColor(QPalette::Background, QColor(240,255,240));
//    this->setPalette(palette);

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
}

void MainWindow::on_Button_manage_clicked()
{
    ManagementWindow * managementWindow = new ManagementWindow(userid, permissionid);
    managementWindow->show();
}

void MainWindow::updateImage(){
    //qDebug() << ii;

    //ui->label_image->setPixmap(QPixmap::fromImage(*image_list.at(ii)));
    //ii = (ii+1)%image_list.size();
}

void MainWindow::on_Button_start_clicked()
{
    QMutexLocker locker(&start_mutex);
    if(!started){
        started = true;
        QString dep = ui->lineEdit_department->text();
        QString ser = ui->lineEdit_serialnumber->text();
        QString manager_name = ui->lineEdit_manage->text();
        QNetworkRequest request;
        QString url_str;
        QDateTime current_time = ui->dateEdit->dateTime();
        QString StrCurrentTime = current_time.toString("yyyyMMdd");
        url_str.append("http://120.78.190.36:9601/begin?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&date=");
        url_str.append(StrCurrentTime);
        url_str.append("&department=");
        url_str.append(dep);
        url_str.append("&serialnumber=");
        url_str.append(ser);
        url_str.append("&manager=");
        url_str.append(manager_name);
        qDebug() << url_str;
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Begin_Reply(QNetworkReply*)));
        ui->lineEdit_department->setEnabled(false);
        ui->lineEdit_serialnumber->setEnabled(false);
        ui->lineEdit_manage->setEnabled(false);
        ui->dateEdit->setEnabled(false);
        ui->Button_start->setText(QString::fromLocal8Bit("结  束"));
    }
    else{
        ui->lineEdit_department->setEnabled(true);
        ui->lineEdit_serialnumber->setEnabled(true);
        ui->lineEdit_manage->setEnabled(true);
        ui->dateEdit->setEnabled(true);
        ui->Button_start->setText(QString::fromLocal8Bit("开  始"));
        isRunning = false;
        started = false;
    }
}

void MainWindow::on_New_Message(bool stable, QString message0, QString message1, long image){
    QMutexLocker locker(&message_mutex);
    ui->label_image->setText(QString::number(image));
    qDebug() << ready_for_current;
    if(ready_for_current){
        if(isRunning && started && stable && unstablePassed){                 //如果正在运行 且 结果稳定
                new_message = true;                         //开始处理当前数据，处理完后才能开始下一次处理
                ready_for_current = false;
                current_code = message0;
                current_no = message1;
                ui->lineEdit_coderesult->setText(current_code);
                ui->lineEdit_noresult->setText(current_no);
                unstablePassed = false;
                if(!ui->lineEdit_coderesult->isEnabled())
                    ui->lineEdit_coderesult->setEnabled(true);
                if(!ui->lineEdit_noresult->isEnabled())
                    ui->lineEdit_noresult->setEnabled(true);
                qDebug() << 1;
        }
        else{                                               //如果未运行 或 结果不稳定
            ui->lineEdit_coderesult->setText("");
            ui->lineEdit_noresult->setText("");
            ui->lineEdit_coderesult->setEnabled(false);
            ui->lineEdit_noresult->setEnabled(false);
            qDebug() << 2;
        }
        if(!stable){
            unstablePassed = true;
            qDebug() << 3;
        }
    }

}

void MainWindow::on_Query_Result(QNetworkReply* reply){
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
                        bool result = dataObj.toBool();
                        if(result){
                            on_Yes_Rule(userid, versionid, current_code, current_no);
                        }
                        else{

                        }
                    }
                    else if(dataObj.isString()){
                        QString dataStr = dataObj.toString();
                        if(QString::compare("norule", dataStr, Qt::CaseSensitive) == 0){
                            isRunning = false;
                            ui->Button_start->setEnabled(false);
                            YesruleDialog *dialog = new YesruleDialog(userid, versionid, current_code, current_no);
                            connect(dialog, SIGNAL(NoRule(int, int, QString, QString)), this, SLOT(on_No_Rule(int, int, QString, QString)));
                            connect(dialog, SIGNAL(YesRule(int, int, QString, QString)), this, SLOT(on_Yes_Rule(int, int, QString, QString)));
                            dialog->show();
                        }
                    }
                    else{

                    }
                }
            }
        }
    }
}

void MainWindow::on_Yes_Rule(int userid, int versionid, QString code, QString no){

    alert = new AlertDialog();
    connect(alert, SIGNAL(closed()), this, SLOT(on_AlertClosed()));
    alert->show();
}

void MainWindow::on_AlertClosed(){
    ui->Button_start->setEnabled(true);
    ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
    ui->Button_check->setEnabled(true);
    //ui->lineEdit_coderesult->setEnabled(true);
    //ui->lineEdit_noresult->setEnabled(true);
    qDebug() << "on_AlertClosed";
    checking_current = false;
    ready_for_current = true;
}

void MainWindow::on_No_Rule(int userid, int versionid, QString code, QString no){
    ui->Button_start->setEnabled(true);
    ui->Button_start->setText(QString::fromLocal8Bit("开  始"));
    ui->Button_check->setEnabled(false);
    ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
    isRunning = false;
    started = false;
    checking_current = false;
    qDebug() << "on_No_Rule";
    ready_for_current = true;
}

void MainWindow::on_Begin_Reply(QNetworkReply* reply){
    QMutexLocker locker(&start_mutex);
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
                        QJsonObject object = dataObj.toObject();
                        versionid = object.value("versionid").toInt();

                        isRunning = true;
                        if(firstStarted){
                            ui->Button_start->setEnabled(true);
                            ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
                            control->start();
                            firstStarted = false;
                        }
                    }
                    else if(dataObj.isBool()){

                    }
                }
            }
        }
    }
}


void MainWindow::on_Button_check_clicked(){
    QMutexLocker locker(&check_mutex);
    if(isRunning && !checking_current && new_message){
        checking_current = true;
        new_message = false;
        ui->Button_check->setEnabled(false);
        ui->Button_check->setText(QString::fromLocal8Bit("检测中"));
        ui->lineEdit_coderesult->setEnabled(false);
        ui->lineEdit_noresult->setEnabled(false);
        QNetworkRequest request;
        QString url_str;
        url_str.append("http://120.78.190.36:9601/queryrule?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&versionid=");
        url_str.append(QString::number(versionid));
        url_str.append("&invoicecode=");
        url_str.append(current_code);
        url_str.append("&invoicenumber=");
        url_str.append(current_no);
        request.setUrl(QUrl(url_str));
        qDebug() << url_str;
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Query_Result(QNetworkReply*)));
    }

}

void MainWindow::on_lineEdit_coderesult_editingFinished()
{
    if(isRunning && started && ! checking_current){
        new_message = true;
        current_code = ui->lineEdit_coderesult->text();
    }
}

void MainWindow::on_lineEdit_noresult_editingFinished()
{
    if(isRunning && started && ! checking_current){
        new_message = true;
        current_no = ui->lineEdit_noresult->text();
    }
}

