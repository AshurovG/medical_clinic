#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QDialog>

namespace Ui {
class adminmenu;
}

class adminmenu : public QDialog
{
    Q_OBJECT

public:
    explicit adminmenu(QWidget *parent = nullptr);
    ~adminmenu();

signals:
    void firstWindow();

private slots:
    void on_btnPatientList_clicked();

    void on_btnDoctorList_clicked();

    void on_btnWardList_clicked();

    void on_btnAnalysisList_clicked();

    void on_btnSurgeryAndOperatingRoom_clicked();

    void on_btnChangeUser_clicked();

    void on_btnAllRecording_clicked();

private:
    Ui::adminmenu *ui;
};

#endif // ADMINMENU_H
