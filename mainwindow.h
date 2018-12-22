#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "managementform.h"
#include "ocrform.h"
#include "ruleform.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userid, int permissionid, QString username, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_quit_confirm();

    void on_ocr_begin();

    void on_ocr_end();

private:
    Ui::MainWindow *ui;
    QString username;
    QString passwords;
    int permissionid;

    ManagementForm * managementForm;
    OCRForm * ocrForm;
    RuleForm * ruleForm;
};

#endif // MAINWINDOW_H
