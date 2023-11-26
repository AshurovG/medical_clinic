#ifndef RECORDFORDOCTOR_H
#define RECORDFORDOCTOR_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class recordfordoctor;
}

class recordfordoctor : public QDialog
{
    Q_OBJECT

public:
    explicit recordfordoctor(QWidget *parent = nullptr);
    ~recordfordoctor();

private slots:
    void send();
    void sendToPatient();
    void sendToRecording();
    void on_btnBack_clicked();

private:
    Ui::recordfordoctor *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RECORDFORDOCTOR_H
