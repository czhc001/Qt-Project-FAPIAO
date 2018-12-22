#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quitdialog.h"
#include "messagedialog.h"

MainWindow::MainWindow(int userid, int permissionid, QString username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    managementForm = new ManagementForm(userid, permissionid, this);
    ocrForm = new OCRForm(userid, permissionid, username, this);
    connect(ocrForm, SIGNAL(begin()), this, SLOT(on_ocr_begin()));
    connect(ocrForm, SIGNAL(endding()), this, SLOT(on_ocr_end()));

    ruleForm = new RuleForm(userid, permissionid, this);
    ui->stackedWidget->addWidget(managementForm);
    ui->stackedWidget->addWidget(ocrForm);
    ui->stackedWidget->addWidget(ruleForm);
    if(permissionid != 1){
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->stackedWidget->setCurrentWidget(ocrForm);
    }
    else
        ui->stackedWidget->setCurrentWidget(managementForm);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_4->setFocusPolicy(Qt::NoFocus);
    showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(managementForm);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ruleForm);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ocrForm);
}

void MainWindow::on_pushButton_4_clicked()
{
    MessageDialog * quitDialog = new MessageDialog(
                QString::fromLocal8Bit("退出确认"),
                QString::fromLocal8Bit("确认退出程序？"),
                QString::fromLocal8Bit("确 定"),
                QString::fromLocal8Bit("取 消"),
                this);
    connect(quitDialog, SIGNAL(confirm()), this, SLOT(on_quit_confirm()));
    quitDialog->setModal(true);
    quitDialog->show();
}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    if(ev->key() == Qt::Key_Escape){
        MessageDialog * quitDialog = new MessageDialog(
                    QString::fromLocal8Bit("退出确认"),
                    QString::fromLocal8Bit("确认退出程序？"),
                    QString::fromLocal8Bit("确 定"),
                    QString::fromLocal8Bit("取 消"),
                    this);
        connect(quitDialog, SIGNAL(confirm()), this, SLOT(on_quit_confirm()));
        quitDialog->setModal(true);
        quitDialog->show();
        ev->ignore();
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(ocrForm != nullptr){
        ocrForm->stop();
        event->accept();
    }
}

void MainWindow::on_quit_confirm(){
    if(ocrForm != nullptr){
        ocrForm->stop();
    }
    close();
}

void MainWindow::on_ocr_begin(){
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
}

void MainWindow::on_ocr_end(){
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
}

