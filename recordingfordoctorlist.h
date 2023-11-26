#ifndef RECORDINGFORDOCTORLIST_H
#define RECORDINGFORDOCTORLIST_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class recordingfordoctorlist; }
QT_END_NAMESPACE

class recordingfordoctorlist : public QDialog
{
    Q_OBJECT

public:
    recordingfordoctorlist(QWidget *parent = nullptr);
    ~recordingfordoctorlist();

private:
    Ui::recordingfordoctorlist *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();

private slots:
    void on_btnBack_clicked();
};

#endif // RECORDINGFORDOCTORLIST_H
