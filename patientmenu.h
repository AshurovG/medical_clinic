#ifndef PATIENTMENU_H
#define PATIENTMENU_H

#include <QDialog>

namespace Ui {
class patientmenu;
}

class patientmenu : public QDialog
{
    Q_OBJECT

public:
    explicit patientmenu(QWidget *parent = nullptr);
    ~patientmenu();

signals:
    void firstWindow();

private slots:
    void on_btnChangeUser_clicked();
    void on_btnAddRecording_clicked();

    void on_btnAnalysisCosts_clicked();

    void on_btnSurgeriesCosts_clicked();

private:
    Ui::patientmenu *ui;
};

#endif // PATIENTMENU_H
