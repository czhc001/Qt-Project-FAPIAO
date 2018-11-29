#ifndef YESRULEDIALOG_H
#define YESRULEDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class yesruleDialog;
}

class YesruleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YesruleDialog(int userid, int versionid, QString code, QString no, QWidget *parent = nullptr);
    ~YesruleDialog();

private slots:
    void on_Button_yes_clicked();

    void on_Button_no_clicked();

    void on_textEdit_textChanged();

    void on_Yes(QNetworkReply*);

    void on_No(QNetworkReply*);

    void on_TimerClose();

private:
    Ui::yesruleDialog *ui;
    int userid;
    int versionid;
    QString code;
    QString no;
    QTimer *timer;

signals:
    void NoRule(int userid, int versionid, QString code, QString no);
    void YesRule(int userid, int versionid, QString code, QString no);
};

#endif // YESRULEDIALOG_H
