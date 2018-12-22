#ifndef MANAGEMENTFORM_H
#define MANAGEMENTFORM_H

#include <QWidget>
#include <adduserdialog.h>
#include <QStandardItemModel>
#include <QtNetwork>
#include "rrdialog.h"

namespace Ui {
class ManagementForm;
}

class ManagementForm : public QWidget
{
    Q_OBJECT

public:
    explicit ManagementForm(int userid, int permissionid, QWidget *parent = nullptr);
    ~ManagementForm();

private slots:
    void on_Button_addUser_clicked();
    void addedNewUser(QString);
    void deletedUser(QNetworkReply*);
    void obtainAllUser(QNetworkReply*);
    void modifiedUser(QString);

    void on_Button_deleteUser_clicked();
    void onCurrentRowChanged(const QModelIndex &, const QModelIndex &);
    void on_Button_modify_clicked();
    void on_Yes_Remove();

private:
    Ui::ManagementForm *ui;
    AddUserDialog * addUserDialog;
    QStandardItemModel* dataModel;
    int userid;
    int permissionid;
    void queryAllUser();
    void tryDeleteUser(int userid);
    QMutex mutex;
    int to_delete_item_count;
    const int USERNAME_COLUMN = 0;
    const int PERMISSIONID_COLUMN = 1;
    const int ID_COLUMN = 3;
    const int PASSWORDS_COLUMN = 2;
    RRDialog *rdialog;
    void hid();
};

#endif // MANAGEMENTFORM_H
