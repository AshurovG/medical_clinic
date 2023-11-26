#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <adminmenu.h>
#include <doctormenu.h>
#include <patientmenu.h>

QT_BEGIN_NAMESPACE
namespace Ui { class authorization; }
QT_END_NAMESPACE

class authorization : public QDialog
{
    Q_OBJECT

public:
    authorization(QWidget *parent = nullptr);
    ~authorization();

private slots:
    void on_btnEnter_clicked();

private:
    Ui::authorization *ui;
    QSqlDatabase dbconn;
    adminmenu *admin_window;
    doctormenu *doctor_window;
    patientmenu *patient_window;
    QLabel *m_errorLabel;

protected:
//    void showEvent(QShowEvent *event) override;
};
#endif
