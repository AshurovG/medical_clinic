#ifndef ANALYSISCOST_H
#define ANALYSISCOST_H

#include <QDialog>
#include <QSqlDatabase>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class analysiscost; }
QT_END_NAMESPACE

class analysiscost : public QDialog
{
    Q_OBJECT

public:
    analysiscost(QWidget *parent = nullptr);
    ~analysiscost();

private:
    Ui::analysiscost *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();
    void search();

private slots:
    void on_btnBack_clicked();
};

#endif // ANALYSISCOST_H
