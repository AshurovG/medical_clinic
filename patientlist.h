#ifndef PATIENTLIST_H
#define PATIENTLIST_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class patientList; }
QT_END_NAMESPACE

class patientList : public QDialog
{
    Q_OBJECT

public:
    patientList(QWidget *parent = nullptr);
    ~patientList();

private:
    Ui::patientList *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();
    void add();
    void del();
    void update();
    void selectedRow();
private slots:
    void on_btnClear_clicked();
    void on_btnBack_clicked();
};

#endif // PATIENTLIST_H
