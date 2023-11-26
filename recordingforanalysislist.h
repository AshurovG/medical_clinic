#ifndef RECORDINGFORANALYSISLIST_H
#define RECORDINGFORANALYSISLIST_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class recordingforanalysislist; }
QT_END_NAMESPACE

class recordingforanalysislist : public QDialog
{
    Q_OBJECT

public:
    recordingforanalysislist(QWidget *parent = nullptr);
    ~recordingforanalysislist();

private:
    Ui::recordingforanalysislist *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();

private slots:
    void on_btnBack_clicked();
};

#endif // RECORDINGFORANALYSISLIST_H
