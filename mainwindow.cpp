#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "managementwindow.h"
#include "yesruledialog.h"
#include <QFileDialog>
#include <QDateTime>
//#include "alertdialog.h"

MainWindow::MainWindow(int userid, int permissionid, QString username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->userid = userid;
    this->username = username;
    this->permissionid = permissionid;
    ui->setupUi(this);

    if(permissionid != 1)
        ui->Button_manage->setEnabled(false);

    control = new MyController();
    control->setImageSize(ui->label_image->size());
    connect(control, SIGNAL(Message(QString, QString, QPixmap, bool)), this, SLOT(on_New_Message(QString, QString, QPixmap, bool)));
    isRunning = false;
    checking_current = false;
    unstablePassed = true;
    ready_for_current = true;

    current_count = 1;

    manage_opened = false;

    ui->label_no_0->setText(QString::fromLocal8Bit("第"));
    ui->label_no_1->setText(QString::fromLocal8Bit("张"));
    ui->label_no_0->setVisible(false);
    ui->label_no_1->setVisible(false);
    QDateTime dateTime = QDateTime::currentDateTime();
    QRegExp wx_code("[0-9]{12}");
    QRegExp wx_no("[0-9]{8}");
    QRegExpValidator *latitude = new QRegExpValidator(wx_code, this);
    QRegExpValidator *latitude_ = new QRegExpValidator(wx_no, this);
    ui->lineEdit_coderesult->setValidator(latitude);
    ui->lineEdit_noresult->setValidator(latitude_);
    ui->lineEdit_coderesult->setEnabled(false);
    ui->lineEdit_noresult->setEnabled(false);
    ui->Button_check->setEnabled(false);
    ui->dateEdit->setDateTime(dateTime);
    initializa_UI();
    ui->lineEdit_coderesult->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_noresult->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lineEdit_serialnumber->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->dateEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    setTabOrder(ui->dateEdit, ui->lineEdit_manage);
    setTabOrder(ui->lineEdit_manage, ui->lineEdit_department);
    setTabOrder(ui->lineEdit_department, ui->lineEdit_serialnumber);
    setTabOrder(ui->lineEdit_serialnumber, ui->Button_start);
    setTabOrder(ui->Button_start, ui->Button_check);
    setTabOrder(ui->Button_check, ui->Button_manage);

    ui->Button_check->setFocusPolicy(Qt::NoFocus);
    ui->Button_manage->setFocusPolicy(Qt::NoFocus);
    ui->Button_start->setFocusPolicy(Qt::NoFocus);

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_AlertClosed()));
}

MainWindow::~MainWindow()
{
    delete control;
    delete ui;
}

void MainWindow::initializa_UI(){
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(25,25,25));
    palette.setColor(QPalette::WindowText, QColor(220,220,220));
    ui->label_image->setPalette(palette);
    QFont font("Microsoft YaHei", 25, 50);
    ui->label_image->setFont(font);


    QFont font0("Microsoft YaHei", 17, 10);
    ui->label_result->setFont(font0);

    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

}

void MainWindow::keyReleaseEvent(QKeyEvent *ev){
    if(ev->key() == Qt::Key_Space){
        if(ui->Button_check->isEnabled())
            on_Button_check_clicked();
    }
    else if(ev->key() == Qt::Key_M){
        if(ui->Button_manage->isEnabled())
            on_Button_manage_clicked();
    }
    else if(ev->key() == Qt::Key_Return){
        if(ui->lineEdit_coderesult->hasFocus() && ui->lineEdit_coderesult->text().size() == 12)
            ui->lineEdit_noresult->setFocus();
        else if(ui->lineEdit_noresult->hasFocus() && ui->lineEdit_noresult->text().size() == 8)
            on_Button_check_clicked();
    }
}

void MainWindow::on_Button_manage_clicked()
{
    QMutexLocker locker(&manage_mutex);
    if(!manage_opened){
        managementWindow = new ManagementWindow(userid, permissionid);
        connect(managementWindow, SIGNAL(windowClosed()), this, SLOT(on_Manage_Closed()));
        managementWindow->show();
        manage_opened = true;
    }
    else{
        if(managementWindow != nullptr){
            managementWindow->activateWindow();
            managementWindow->raise();
        }
    }
}

void MainWindow::on_Manage_Closed(){
    manage_opened = false;
}

void MainWindow::on_Button_start_clicked()
{
    QMutexLocker locker(&start_mutex);

    if(!isRunning){
        if(ui->lineEdit_department->text().isEmpty() || ui->lineEdit_manage->text().isEmpty() || ui->lineEdit_serialnumber->text().isEmpty()){
            ui->label_hint->setText(QString::fromLocal8Bit("请补全信息"));
            return;
        }
        if(ui->lineEdit_manage->text().size() > 20){
            ui->label_hint->setText(QString::fromLocal8Bit("经办人名过长"));
            return;
        }
        if(ui->lineEdit_department->text().size() > 20){
            ui->label_hint->setText(QString::fromLocal8Bit("部门名过长"));
            return;
        }
        if(ui->lineEdit_serialnumber->text().size() > 20){
            ui->label_hint->setText(QString::fromLocal8Bit("编号过长"));
            return;
        }
        ui->label_hint->setText("");
        isRunning = true;
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
        ui->label_image->setText(QString::fromLocal8Bit("  初始化中..."));
    }
    else{
        qDebug() << "STOP";
        stop();
    }
}

void MainWindow::on_New_Message(QString message0, QString message1, QPixmap image, bool stable){
    QMutexLocker locker(&message_mutex);
    if(!isRunning)
        return;
    if(stable)
        qDebug() << "STABLE!!!";
    if(ready_for_current){
        ui->label_image->setPixmap(image);
        //qDebug() << isRunning << " " << " " << stable << " " << unstablePassed;
        if(isRunning && stable && unstablePassed){                  //如果正在运行 且 结果稳定
                ready_for_current = false;                          //开始处理当前数据，处理完后才能开始下一次处理
                current_code = message0;
                current_no = message1;
                ui->lineEdit_coderesult->setText(current_code);
                ui->lineEdit_noresult->setText(current_no);
                unstablePassed = false;
                //if(!ui->lineEdit_coderesult->isEnabled())
                //    ui->lineEdit_coderesult->setEnabled(true);
                //if(!ui->lineEdit_noresult->isEnabled())
                //    ui->lineEdit_noresult->setEnabled(true);
                ui->Button_check->setEnabled(true);
                //qDebug() << 1;
        }
        else{                                               //如果未运行 或 结果不稳定
            ui->lineEdit_coderesult->setText("");
            ui->lineEdit_noresult->setText("");
            //ui->lineEdit_coderesult->setEnabled(false);
            //ui->lineEdit_noresult->setEnabled(false);
            //qDebug() << 2;
        }
        if(!stable){
            unstablePassed = true;
            //qDebug() << 3;
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
                            YesruleDialog *dialog = new YesruleDialog(userid, versionid, current_code, current_no, this);
                            connect(dialog, SIGNAL(NoRule(int, int, QString, QString)), this, SLOT(on_No_Rule(int, int, QString, QString)));
                            connect(dialog, SIGNAL(YesRule(int, int, QString, QString)), this, SLOT(on_Yes_Rule(int, int, QString, QString)));
                            dialog->setModal(true);
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
    //alert = new AlertDialog(this);
    //connect(alert, SIGNAL(closed()), this, SLOT(on_AlertClosed()));
    //alert->setModal(true);
    //alert->show();

    QPalette palette0;
    palette0.setColor(QPalette::WindowText, Qt::green);
    ui->label_result->setPalette(palette0);
    ui->label_result->setText(QString::fromLocal8Bit("合  规"));
    ++current_count;
    timer.start(500);
}


void MainWindow::on_AlertClosed(){
    timer.stop();
    ui->label_no->setText(QString::number(current_count));
    ui->label_result->clear();
    ui->Button_start->setEnabled(true);
    ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
    ui->Button_check->setEnabled(false);
    ui->lineEdit_coderesult->setEnabled(true);
    ui->lineEdit_noresult->setEnabled(true);
    qDebug() << "on_AlertClosed";
    checking_current = false;
    ready_for_current = true;
    isRunning = true;
}

void MainWindow::on_No_Rule(int userid, int versionid, QString code, QString no){
    stop();
}

void MainWindow::stop(){
    control->stop();
    qDebug() << "ocr stop";
    ui->label_image->clear();
    ui->lineEdit_department->setEnabled(true);
    ui->lineEdit_serialnumber->setEnabled(true);
    ui->lineEdit_manage->setEnabled(true);
    ui->dateEdit->setEnabled(true);
    ui->Button_check->setEnabled(false);
    ui->Button_check->setText(QString::fromLocal8Bit("检测合规"));
    ui->Button_start->setText(QString::fromLocal8Bit("开  始"));
    ui->Button_start->setEnabled(true);
    ui->lineEdit_noresult->setText("");
    ui->lineEdit_coderesult->setText("");
    ui->lineEdit_coderesult->setEnabled(false);
    ui->lineEdit_noresult->setEnabled(false);

    ui->label_no_0->setVisible(false);
    ui->label_no_1->setVisible(false);
    ui->label_no->setVisible(false);
    isRunning = false;
    checking_current = false;
    unstablePassed = true;
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
                        current_count = 1;
                        ui->Button_start->setEnabled(true);
                        ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
                        ui->label_no_0->setVisible(true);
                        ui->label_no_1->setVisible(true);
                        ui->label_no->setText(QString::number(1));
                        ui->label_no->setVisible(true);
                        ui->lineEdit_coderesult->setEnabled(true);
                        ui->lineEdit_noresult->setEnabled(true);
                        control->start();
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
    if(isRunning && !checking_current){
        current_code = ui->lineEdit_coderesult->text();
        current_no = ui->lineEdit_noresult->text();
        if(current_code.size() != 12 || current_no.size() != 8){
            QPalette palette0;
            palette0.setColor(QPalette::WindowText, Qt::red);
            ui->label_result->setPalette(palette0);
            ui->label_result->setText(QString::fromLocal8Bit("发票代码或号码格式错误"));
            return;
        }
        ui->label_result->setText("");
        checking_current = true;
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
    return;
    qDebug() << "EF0";
    if(isRunning  && ! checking_current){
        current_code = ui->lineEdit_coderesult->text();
        if(ui->lineEdit_coderesult->hasFocus())
            ui->lineEdit_noresult->setFocus();
    }
}

void MainWindow::on_lineEdit_noresult_editingFinished()
{
    return;
    qDebug() << "EF1";
    if(isRunning  && ! checking_current){
        current_no = ui->lineEdit_noresult->text();
        if(ui->lineEdit_noresult->hasFocus())
            on_Button_check_clicked();
    }
}

void MainWindow::on_lineEdit_serialnumber_returnPressed()
{
    on_Button_start_clicked();
}

void MainWindow::on_lineEdit_manage_returnPressed()
{
    if(ui->lineEdit_manage->text().size() > 0)
        ui->lineEdit_department->setFocus();
}

void MainWindow::on_lineEdit_department_returnPressed()
{
    if(ui->lineEdit_department->text().size() > 0)
        ui->lineEdit_serialnumber->setFocus();
}
