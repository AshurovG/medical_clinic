#ifndef RECORDINGFORWARDLIST_H
#define RECORDINGFORWARDLIST_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class recordingforwardlist; }
QT_END_NAMESPACE

class recordingforwardlist : public QDialog
{
    Q_OBJECT

public:
    recordingforwardlist(QWidget *parent = nullptr);
    ~recordingforwardlist();

private:
    Ui::recordingforwardlist *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();

private slots:
    void on_btnBack_clicked();
};

#endif // RECORDINGFORWARDLIST_H
