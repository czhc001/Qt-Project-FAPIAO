#include "ocrform.h"
#include "ui_ocrform.h"
#include "managementwindow.h"
#include "yesruledialog.h"
#include <QFileDialog>
#include <QDateTime>
#include <QStackedWidget>
#include <quitdialog.h>
#include <messagedialog.h>
#include <confirmdialog.h>
#include <severaddress.h>

OCRForm::OCRForm(int userid, int permissionid, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCRForm)
{
    setAttribute(Qt::WA_DeleteOnClose);

    this->userid = userid;
    this->username = username;
    this->permissionid = permissionid;
    ui->setupUi(this);

    //if(permissionid != 1)
    //    ui->Button_manage->setEnabled(false);

    control = new MyController();
    //control->setImageSize(label_image->size());
    connect(control, SIGNAL(Message(QString, QString, QPixmap, bool)), this, SLOT(on_New_Message(QString, QString, QPixmap, bool)));
    connect(control, SIGNAL(errorMessage(int)), this, SLOT(on_Error_Message(int)));
    isRunning = false;
    checking_current = false;
    unstablePassed = true;
    ready_for_current = true;

    current_count = 1;

    manage_opened = false;
    camera_disconnected = false;

    ui->label_no_0->setText(QString::fromLocal8Bit("张"));
    ui->label_no_1->setText(QString::fromLocal8Bit("第"));
    ui->label_no_0->setVisible(false);
    ui->label_no_1->setVisible(false);
    QDateTime dateTime = QDateTime::currentDateTime();
    QRegExp wx_code("[0-9]{12}");
    QRegExp wx_no("[0-9]{8}");
    QRegExp wx_count("[0-9]{8}");
    QRegExpValidator *latitude = new QRegExpValidator(wx_code, this);
    QRegExpValidator *latitude_ = new QRegExpValidator(wx_no, this);
    QRegExpValidator *latitude__ = new QRegExpValidator(wx_count, this);
    ui->lineEdit_totalcount->setValidator(latitude__);
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

    label_camera_alert->setVisible(false);
    ui->Button_continue->setVisible(false);
    ui->Button_continue->setEnabled(false);

    setTabOrder(ui->dateEdit, ui->lineEdit_manage);
    setTabOrder(ui->lineEdit_manage, ui->lineEdit_department);
    setTabOrder(ui->lineEdit_department, ui->lineEdit_totalcount);
    setTabOrder(ui->lineEdit_totalcount, ui->lineEdit_serialnumber);
    setTabOrder(ui->lineEdit_serialnumber, ui->Button_start);
    setTabOrder(ui->Button_start, ui->Button_check);
    //setTabOrder(ui->Button_check, ui->Button_manage);

    ui->Button_check->setFocusPolicy(Qt::NoFocus);
    //ui->Button_manage->setFocusPolicy(Qt::NoFocus);
    ui->Button_start->setFocusPolicy(Qt::NoFocus);
    ui->Button_continue->setFocusPolicy(Qt::NoFocus);
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_AlertClosed()));
}

OCRForm::~OCRForm()
{
    delete control;
    delete ui;
}

void OCRForm::initializa_UI(){

    stackedlayout = new QStackedLayout();
    label_image = new QLabel();
    label_init = new QLabel();
    label_camera_alert = new QLabel();

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(25,25,25));
    label_image->setPalette(palette);
    label_image->setAutoFillBackground(true);

    QFont font("Microsoft YaHei", 25, 50);
    label_image->setFont(font);
    label_init->setFont(font);
    label_camera_alert->setFont(font);

    QPalette palette1;
    palette1.setColor(QPalette::WindowText, QColor(220,220,220));
    label_init->setPalette(palette1);

    QPalette palette2;
    palette2.setColor(QPalette::WindowText, QColor(255,20,20));
    label_camera_alert->setPalette(palette2);
    label_camera_alert->setText(QString::fromLocal8Bit("  摄像头未连接，请连接后点击 开启摄像"));

    QFont font0("Microsoft YaHei", 17, 10);
    ui->label_result->setFont(font0);

    //setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //setFixedSize(this->width(), this->height());



    QSizePolicy sizePolicy0(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label_image->setSizePolicy(sizePolicy0);
    label_init->setSizePolicy(sizePolicy0);
    label_camera_alert->setSizePolicy(sizePolicy0);

    label_image->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    label_init->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    label_camera_alert->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    label_image->setScaledContents(true);

    stackedlayout->setObjectName("stackedLayout");
    stackedlayout->addWidget(label_image);
    stackedlayout->addWidget(label_init);
    stackedlayout->addWidget(label_camera_alert);

    stackedlayout->setStackingMode(QStackedLayout::StackAll);
    ui->verticalLayout_10->insertLayout(0, stackedlayout);
    ui->verticalLayout_10->setStretch(0, 10);
    ui->verticalLayout_10->setStretch(1, 3);

    ui->verticalLayout_9->setStretch(0, 1);
    ui->verticalLayout_9->setStretch(1, 3);
    ui->verticalLayout_9->setStretch(2, 1);

    //stackedlayout->setCurrentIndex(2);
}

void OCRForm::keyReleaseEvent(QKeyEvent *ev){
    if(ev->key() == Qt::Key_Space){
        if(ui->Button_check->isEnabled())
            on_Button_check_clicked();
    }
    else if(ev->key() == Qt::Key_M){
        //if(ui->Button_manage->isEnabled())
        //    on_Button_manage_clicked();
    }
    else if(ev->key() == Qt::Key_Return){
        if(ui->lineEdit_coderesult->hasFocus() && ui->lineEdit_coderesult->text().size() == 12)
            ui->lineEdit_noresult->setFocus();
        else if(ui->lineEdit_noresult->hasFocus() && ui->lineEdit_noresult->text().size() == 8)
            on_Button_check_clicked();
        else if(ui->lineEdit_totalcount->hasFocus() && ui->lineEdit_totalcount->text().size() > 0)
            ui->lineEdit_serialnumber->setFocus();
    }
}
/*
void OCRForm::on_Button_manage_clicked()
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
*/
void OCRForm::on_Manage_Closed(){
    manage_opened = false;
}

void OCRForm::on_Button_start_clicked()
{
    QMutexLocker locker(&start_mutex);

    if(!isRunning){
        if(ui->lineEdit_department->text().isEmpty() || ui->lineEdit_manage->text().isEmpty() || ui->lineEdit_serialnumber->text().isEmpty() || ui->lineEdit_totalcount->text().isEmpty()){
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

        emit begin();
        ui->label_hint->setText("");
        isRunning = true;
        department = ui->lineEdit_department->text();
        serial_number = ui->lineEdit_serialnumber->text();
        manager_name = ui->lineEdit_manage->text();
        totalcount = ui->lineEdit_totalcount->text().toInt();
        QNetworkRequest request;
        QString url_str;
        current_time = ui->dateEdit->dateTime();
        QString StrCurrentTime = current_time.toString("yyyyMMdd");
        url_str.append("http://" + SeverAddress::address + "/begin?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&date=");
        url_str.append(StrCurrentTime);
        url_str.append("&department=");
        url_str.append(department);
        url_str.append("&serialnumber=");
        url_str.append(serial_number);
        url_str.append("&manager=");
        url_str.append(manager_name);
        url_str.append("&totalnumber=");
        url_str.append(QString::number(totalcount));
        qDebug() << url_str;
        request.setUrl(QUrl(url_str));
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Begin_Reply(QNetworkReply*)));
        ui->lineEdit_department->setEnabled(false);
        ui->lineEdit_serialnumber->setEnabled(false);
        ui->lineEdit_manage->setEnabled(false);
        ui->lineEdit_totalcount->setEnabled(false);
        ui->dateEdit->setEnabled(false);
        ui->Button_start->setText(QString::fromLocal8Bit("结  束"));
        label_init->setText(QString::fromLocal8Bit("  初始化中..."));
    }
    else{
        qDebug() << "STOP";
        QString title = QString::fromLocal8Bit("结 束");
        QString message = QString::fromLocal8Bit("请选择您的操作");
        QString b1 = QString::fromLocal8Bit("本版全部结束");
        QString b2 = QString::fromLocal8Bit("本版暂停");
        MessageDialog * messageDialog = new MessageDialog(title, message, b1, b2);
        connect(messageDialog, SIGNAL(confirm()), this, SLOT(on_stop_button()));
        connect(messageDialog, SIGNAL(cancel()), this, SLOT(on_pause_button()));
        messageDialog->setModal(true);
        messageDialog->show();
        //complete();
        //stop();
    }
}

void OCRForm::on_stop_button(){
    complete();
    stop();
}

void OCRForm::on_pause_button(){
    stop();
}

void OCRForm::on_Error_Message(int a){
    label_camera_alert->setVisible(true);
    label_init->setVisible(false);
    ui->Button_continue->setVisible(true);
    ui->Button_continue->setEnabled(true);
    camera_disconnected = true;
}

void OCRForm::on_New_Message(QString message0, QString message1, QPixmap image, bool stable){
    QMutexLocker locker(&message_mutex);
    if(!isRunning)
        return;
    if(label_camera_alert->isVisible())
        label_camera_alert->setVisible(false);
    if(label_init->isVisible())
        label_init->setVisible(false);
    if(camera_disconnected){
        ui->Button_continue->setEnabled(false);
        ui->Button_continue->setVisible(false);
        camera_disconnected = false;
    }

    if(stable)
        qDebug() << "STABLE!!!";
    if(ready_for_current){

        //image = image.scaled(label_image->contentsRect().size(), Qt::KeepAspectRatio);
        int label_image_height = label_image->contentsRect().size().height();
        int label_image_width = label_image->contentsRect().size().width();
        int img_height = image.size().height();
        int img_width = image.size().width();
        double label_hTw = (double)label_image_height/(double)label_image_width;
        double img_hTw = (double)img_height/(double)img_width;

        if( img_hTw > label_hTw){ //图像更窄,需要截取
            int height_scaled = img_width * label_hTw;
            int start_x = 0;
            int start_y = (img_height - height_scaled)/2;
            image = image.copy(start_x, start_y, img_width, height_scaled);
            image = image.scaled(label_image->contentsRect().size(), Qt::KeepAspectRatio);
        }
        else{
            //image = image.scaled(label_image->contentsRect().size(), Qt::KeepAspectRatio);
        }

        label_image->setPixmap(image);
        //qDebug() << isRunning << " " << " " << stable << " " << unstablePassed;
        if(isRunning && stable && unstablePassed){                  //如果正在运行 且 结果稳定
                ready_for_current = false;                          //开始处理当前数据，处理完后才能开始下一次处理
                current_code = message0;
                current_no = message1;
                if(current_code.size() == 12)
                    ui->lineEdit_coderesult->setText(current_code);
                if(current_no.size() == 8)
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
            //ui->lineEdit_coderesult->setText("");
            //ui->lineEdit_noresult->setText("");
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

void OCRForm::on_Query_Result(QNetworkReply* reply){
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

                    if(dataObj.isString()){
                        QString dataStr = dataObj.toString();
                        if(QString::compare("norule", dataStr, Qt::CaseSensitive) == 0){
                            QJsonValue info = Object.value("information");
                            QJsonArray infoArray = info.toArray();
                            /*
                            QString history_manager = infoObj.value("manager").toString();
                            QString history_department = infoObj.value("department").toString();
                            QString history_date = infoObj.value("date").toString();
                            QDateTime history_datetime = QDateTime::fromString(history_date, "yyyyMMdd");
                            QString history_serial = infoObj.value("serialnumber").toString();
                            QString history_username = infoObj.value("username").toString();
                            QString history_code = infoObj.value("invoicecode").toString();
                            QString history_no = infoObj.value("invoicenumber").toString();
                            */
                            YesruleDialog *dialog = new YesruleDialog(userid, versionid, current_count, current_code, current_no, this->manager_name,
                                                                      this->department, this->current_time.toString("yyyy/MM/dd"), this->serial_number,
                                                                      this->username, infoArray, this);
                            connect(dialog, SIGNAL(NoRule(int, int, QString, QString)), this, SLOT(on_No_Rule(int, int, QString, QString)));
                            connect(dialog, SIGNAL(YesRule(int, int, QString, QString)), this, SLOT(on_Yes_Rule(int, int, QString, QString)));
                            connect(dialog, SIGNAL(Complete(int, int, QString, QString)), this, SLOT(on_Complete(int, int, QString, QString)));
                            dialog->setModal(true);
                            dialog->show();
                        }
                    }
                    else if(dataObj.isObject()){
                        QJsonObject obj = dataObj.toObject();
                        int flag = obj.value("flag").toInt();
                        if(1 == flag){
                            on_Yes_Rule(userid, versionid, current_code, current_no);
                        }
                        else if(2 == flag){
                            QString title = QString::fromLocal8Bit("本版结束");
                            QString message = QString::fromLocal8Bit("本版共") + QString::number(current_count) + QString::fromLocal8Bit("张，已全部检测通过");
                            ConfirmDialog * confirmDialog = new ConfirmDialog(title, message, this);
                            confirmDialog->setModal(true);
                            confirmDialog->show();
                            stop();
                        }
                    }
                    else{
                        QString title = QString::fromLocal8Bit("服务器错误");
                        QString message = QString::fromLocal8Bit("服务器数据库查询出错");
                        ConfirmDialog * confirmDialog = new ConfirmDialog(title, message, this);
                        confirmDialog->setModal(true);
                        confirmDialog->show();
                        stop();
                    }
                }
            }
        }
    }
}

void OCRForm::on_Yes_Rule(int userid, int versionid, QString code, QString no){
    //alert = new AlertDialog(this);
    //connect(alert, SIGNAL(closed()), this, SLOT(on_AlertClosed()));
    //alert->setModal(true);
    //alert->show();

    QPalette palette0;
    palette0.setColor(QPalette::WindowText, Qt::green);
    ui->label_result->setPalette(palette0);
    ui->label_result->setText(QString::fromLocal8Bit("合  规"));
    ui->Button_check->setEnabled(true);
    ++current_count;
    timer.start(500);
}

void OCRForm::on_Complete(int userid, int versionid, QString code, QString no){
    QString title = QString::fromLocal8Bit("本版完成");
    QString content = QString::fromLocal8Bit("本版共") + QString::number(current_count) + QString::fromLocal8Bit("张，已全部检查通过");
    ++current_count;
    complete();
    stop();

    ConfirmDialog * confirmDialog = new ConfirmDialog(title, content, this);
    confirmDialog->setModal(true);
    confirmDialog->show();
}


void OCRForm::on_AlertClosed(){
    timer.stop();
    ui->label_no->setText(QString::number(current_count));
    ui->label_result->clear();
    ui->Button_start->setEnabled(true);
    ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
    //ui->Button_check->setEnabled(false);
    ui->lineEdit_coderesult->setEnabled(true);
    ui->lineEdit_noresult->setEnabled(true);
    ui->lineEdit_coderesult->clear();
    ui->lineEdit_noresult->clear();
    ui->lineEdit_coderesult->setFocus();
    qDebug() << "on_AlertClosed";
    checking_current = false;
    ready_for_current = true;
    isRunning = true;
}

void OCRForm::on_No_Rule(int userid, int versionid, QString code, QString no){
    stop();
}

void OCRForm::stop(){
    control->stop();
    qDebug() << "ocr stop";
    label_image->clear();
    label_init->setVisible(false);
    label_camera_alert->setVisible(false);
    ui->lineEdit_department->setEnabled(true);
    ui->lineEdit_serialnumber->setEnabled(true);
    ui->lineEdit_manage->setEnabled(true);
    ui->dateEdit->setEnabled(true);
    ui->lineEdit_totalcount->setEnabled(true);
    ui->Button_check->setEnabled(false);
    ui->Button_check->setText(QString::fromLocal8Bit("检测合规"));
    ui->Button_start->setText(QString::fromLocal8Bit("开  始"));
    ui->Button_start->setEnabled(true);
    ui->lineEdit_noresult->setText("");
    ui->lineEdit_coderesult->setText("");
    ui->lineEdit_coderesult->setEnabled(false);
    ui->lineEdit_noresult->setEnabled(false);
    label_camera_alert->setVisible(false);
    ui->Button_continue->setEnabled(false);
    ui->Button_continue->setVisible(false);

    ui->label_no_0->setVisible(false);
    ui->label_no_1->setVisible(false);
    ui->label_no->setVisible(false);
    isRunning = false;
    checking_current = false;
    unstablePassed = true;
    ready_for_current = true;

    emit endding();
}

void OCRForm::complete(){
    QNetworkRequest request;
    QString url_str;
    url_str.append("http://" + SeverAddress::address + "/conbegin?userid=");
    url_str.append(QString::number(userid));
    url_str.append("&date=");
    url_str.append(current_time.toString("yyyyMMdd"));
    url_str.append("&department=");
    url_str.append(department);
    url_str.append("&serialnumber=");
    url_str.append(serial_number);
    url_str.append("&managerr=");
    url_str.append(manager_name);
    url_str.append("&totalnumber=");
    url_str.append(QString::number(current_count - 1));
    request.setUrl(QUrl(url_str));
    qDebug() << url_str;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->get(request);
}

void OCRForm::on_Begin_Reply(QNetworkReply* reply){
    QMutexLocker locker(&start_mutex);
    QByteArray data = reply->readAll();
    QString result = QString::fromStdString(data.toStdString()).toUtf8();
    qDebug() << result;
    if(result.isEmpty()){
        QString title = QString::fromLocal8Bit("错误");
        QString message = QString::fromLocal8Bit("服务器错误");
        ConfirmDialog * confirmDialog = new ConfirmDialog(title, message, this);
        confirmDialog->setModal(true);
        confirmDialog->show();
        stop();
        return;
    }
    QJsonDocument document;
    QJsonParseError ParseError;
    document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);
    if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
        if(document.isObject()){
            QJsonObject Object = document.object();
            if(!Object.isEmpty()){
                if(Object.contains("data")){
                    QJsonValue dataObj = Object.value("data");
                    if(dataObj.isBool() && !dataObj.toBool()){

                    }
                    else if(dataObj.isObject()){  //判断是否是对象
                        QJsonObject object = dataObj.toObject();
                        int flag = object.value("flag").toInt();

                        if(flag == 1){
                            versionid = object.value("versionid").toInt();
                            isRunning = true;
                            current_count = 1;
                            ui->Button_start->setEnabled(true);

                            checkstart();
                        }
                        else if(flag == -1){
                            //QString message = object.value("message").toString();
                            //ui->label_hint->setText(message);
                            QString title = "  ";
                            QString message = object.value("message").toString();
                            ConfirmDialog * confirmDialog = new ConfirmDialog(title, message, this);
                            connect(confirmDialog, SIGNAL(confirm()), this, SLOT(on_pause_button()));
                            confirmDialog->setModal(true);
                            confirmDialog->show();
                        }
                        else if(flag == 2){
                            current_count = 1;

                            QString message = object.value("message").toString();
                            QString title = QString::fromLocal8Bit("继续确认");
                            MessageDialog * messageDialog = new MessageDialog(title, message,
                                                                              QString::fromLocal8Bit("确 定"),
                                                                              QString::fromLocal8Bit("取 消"),
                                                                              this);
                            connect(messageDialog, SIGNAL(confirm()), this, SLOT(on_continue_confirm()));
                            connect(messageDialog, SIGNAL(cancel()), this, SLOT(on_pause_button()));
                            messageDialog->setModal(true);
                            messageDialog->show();
                        }
                        else if(flag == 3){
                            QString message = object.value("message").toString();
                            int completenumber = object.value("completednumber").toInt();
                            versionid = object.value("versionid").toInt();
                            current_count = completenumber + 1;
                            qDebug() << current_count;
                            ConfirmDialog * confirmDialog = new ConfirmDialog("  ", message, this);
                            confirmDialog->setModal(true);
                            confirmDialog->show();
                            //ui->label_hint->setText(message);
                            checkstart();
                        }
                    }
                }
                else{
                    QString title = QString::fromLocal8Bit("错误");
                    QString message = QString::fromLocal8Bit("服务器错误");
                    ConfirmDialog * confirmDialog = new ConfirmDialog(title, message, this);
                    confirmDialog->setModal(true);
                    confirmDialog->show();
                    stop();
                }
            }
        }
    }
}

void OCRForm::checkstart(){
    ui->label_no_0->setVisible(true);
    ui->label_no_1->setVisible(true);
    ui->label_no->setText(QString::number(current_count));
    ui->label_no->setVisible(true);
    ui->Button_check->setText(QString::fromLocal8Bit("合规检测"));
    ui->Button_check->setEnabled(true);
    ui->lineEdit_coderesult->setEnabled(true);
    ui->lineEdit_noresult->setEnabled(true);
    control->start();
}


void OCRForm::on_Button_check_clicked(){
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

        url_str.append("http://" + SeverAddress::address + "/queryrule?userid=");
        url_str.append(QString::number(userid));
        url_str.append("&versionid=");
        url_str.append(QString::number(versionid));
        url_str.append("&invoicecode=");
        url_str.append(current_code);
        url_str.append("&invoicenumber=");
        url_str.append(current_no);
        url_str.append("&completednumber=");
        url_str.append(QString::number(current_count));
        request.setUrl(QUrl(url_str));
        qDebug() << url_str;
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkReply *reply = manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_Query_Result(QNetworkReply*)));
    }

}

void OCRForm::on_lineEdit_manage_returnPressed()
{
    if(ui->lineEdit_manage->text().size() > 0)
        ui->lineEdit_department->setFocus();
}

void OCRForm::on_lineEdit_department_returnPressed()
{
    if(ui->lineEdit_department->text().size() > 0)
        ui->lineEdit_totalcount->setFocus();
}

void OCRForm::on_lineEdit_serialnumber_returnPressed()
{
    if(ui->lineEdit_serialnumber->text().size() > 0)
        on_Button_start_clicked();
}

void OCRForm::on_Button_continue_clicked()
{
    if(camera_disconnected){
        label_camera_alert->setVisible(false);
        label_init->setVisible(true);
        label_init->setText(QString::fromLocal8Bit("  初始化中..."));
        if(!control->isRunning())
            control->start();
        return;
    }
}

void OCRForm::on_continue_confirm(){
    QNetworkRequest request;
    QString url_str;

    url_str.append("http://" + SeverAddress::address + "/conbegin?userid=");
    url_str.append(QString::number(userid));
    url_str.append("&date=");
    url_str.append(current_time.toString("yyyyMMdd"));
    url_str.append("&department=");
    url_str.append(department);
    url_str.append("&serialnumber=");
    url_str.append(serial_number);
    url_str.append("&managerr=");
    url_str.append(manager_name);
    url_str.append("&totalnumber=");
    url_str.append(QString::number(totalcount));
    request.setUrl(QUrl(url_str));
    qDebug() << url_str;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_confirm_reply(QNetworkReply*)));
}

void OCRForm::on_confirm_reply(QNetworkReply *reply){
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
                    QJsonValue objV = Object.value("data");
                    if(objV.isBool()){
                        ui->label_hint->setText(QString::fromLocal8Bit("开始失败"));
                    }
                    else{
                        QJsonObject obj = objV.toObject();
                        int completenumber = obj.value("completednumber").toInt();
                        current_count = completenumber + 1;
                        versionid = obj.value("versionid").toInt();
                        checkstart();
                    }
                }
            }
        }
    }
}



