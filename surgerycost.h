#ifndef SURGERYCOST_H
#define SURGERYCOST_H

#include <QDialog>
#include <QSqlDatabase>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class surgerycost; }
QT_END_NAMESPACE

class surgerycost : public QDialog
{
    Q_OBJECT

public:
    surgerycost(QWidget *parent = nullptr);
    ~surgerycost();

private:
    Ui::surgerycost *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();
    void search();

private slots:
    void on_btnBack_clicked();
};

#endif // SURGERYCOST_H
