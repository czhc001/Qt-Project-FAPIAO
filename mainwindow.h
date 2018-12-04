#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QObject>
#include <QtNetwork>
#include "mycontroller.h"
#include "alertdialog.h"
//#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userid, int permissionid, QString username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Button_manage_clicked();
    void updateImage();

    void on_Button_start_clicked();
    void on_New_Message(QString, QString, Mat, bool);
    void on_Begin_Reply(QNetworkReply*);
    void on_Query_Result(QNetworkReply*);
    void on_Yes_Rule(int, int, QString, QString);
    void on_No_Rule(int, int, QString, QString);

    void on_Button_check_clicked();

    void on_lineEdit_coderesult_editingFinished();

    void on_lineEdit_noresult_editingFinished();

    void on_AlertClosed();

private:
    QImage MatToQImage(const cv::Mat& mat);
    Ui::MainWindow *ui;
    int userid;
    int permissionid;
    int versionid;
    QString username;
    QString department;
    QString current_code;
    QString current_no;
    QTimer *timer;
    int ii;
    QList<QImage *> image_list;
    bool isRunning;
    bool started;
    bool firstStarted;
    bool checking_current;
    bool new_message;
    bool unstablePassed;
    bool ready_for_current;
    QMutex start_mutex;
    QMutex check_mutex;
    QMutex message_mutex;
    AlertDialog *alert;

    MyController *control;

    void initializa_UI();
};



#endif // MAINWINDOW_H
