#ifndef OCRFORM_H
#define OCRFORM_H

#include <QWidget>
#include <QMutex>
#include <QObject>
#include <QtNetwork>
#include "mycontroller.h"
//#include "alertdialog.h"
#include <QKeyEvent>
#include <QTimer>
#include <managementwindow.h>
#include <QStackedLayout>
#include <QLabel>
#include <QWidget>

namespace Ui {
class OCRForm;
}

class OCRForm : public QWidget
{
    Q_OBJECT

public:
    explicit OCRForm(int userid, int permissionid, QString username, QWidget *parent = 0);
    ~OCRForm();
    void complete();
    void releaseRes();

protected:
    void keyReleaseEvent(QKeyEvent *);

private:
    Ui::OCRForm *ui;
    int userid;
    int permissionid;
    int versionid;
    QString username;
    QString department;
    QString manager_name;
    QString serial_number;
    int totalcount;
    QDateTime current_time;
    QString current_code;
    QString current_no;
    QList<QImage *> image_list;
    bool isRunning;
    bool checking_current;
    bool unstablePassed;
    bool ready_for_current;
    bool camera_disconnected;

    bool manage_opened;

    int current_count;

    QTimer timer;
    QMutex start_mutex;
    QMutex check_mutex;
    QMutex message_mutex;
    QMutex manage_mutex;
    //AlertDialog *alert;

    MyController *control;
    ManagementWindow * managementWindow;

    void initializa_UI();
    void checkstart();
    void stop();


    QStackedLayout * stackedlayout;
    QLabel * label_camera_alert;
    QLabel * label_image;
    QLabel * label_init;

private slots:
    //void on_Button_manage_clicked();

    void on_Button_start_clicked();
    void on_New_Message(QString, QString, QPixmap, bool);
    void on_Begin_Reply(QNetworkReply*);
    void on_Query_Result(QNetworkReply*);
    void on_Yes_Rule(int, int, QString, QString);
    void on_No_Rule(int, int, QString, QString);
    void on_Complete(int, int, QString, QString);

    void on_Button_check_clicked();

    void on_AlertClosed();

    void on_Manage_Closed();

    void on_lineEdit_serialnumber_returnPressed();

    void on_lineEdit_manage_returnPressed();

    void on_lineEdit_department_returnPressed();

    void on_Error_Message(int);

    void on_Button_continue_clicked();

    void on_continue_confirm();

    void on_confirm_reply(QNetworkReply*);

    void on_stop_button();

    void on_pause_button();

signals:
    void begin();
    void endding();
};

#endif // OCRFORM_H
