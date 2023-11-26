#ifndef RECORDINGFORSURGERY_H
#define RECORDINGFORSURGERY_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class recordingforsurgery; }
QT_END_NAMESPACE

class recordingforsurgery : public QDialog
{
    Q_OBJECT

public:
    recordingforsurgery(QWidget *parent = nullptr);
    ~recordingforsurgery();

private:
    Ui::recordingforsurgery *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();

private slots:
    void on_btnBack_clicked();
};


#endif // RECORDINGFORSURGERY_H
