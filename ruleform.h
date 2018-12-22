#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>
#include <QNetworkReply>

namespace Ui {
class RuleForm;
}

class RuleForm : public QWidget
{
    Q_OBJECT

public:
    explicit RuleForm(int userid, int permissionid, QWidget *parent = 0);
    ~RuleForm();

private slots:
    void on_pushButton_clicked();
    void on_Reply(QNetworkReply*);

    void on_pushButton_2_clicked();
    void on_Reply2(QNetworkReply*);

private:
    Ui::RuleForm *ui;
    int userid;
    int permissionid;
    int local_n;
};

#endif // RULEFORM_H
