#ifndef ANALYSISLIST_H
#define ANALYSISLIST_H

#include <QDialog>
#include <QSqlDatabase>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class analysislist; }
QT_END_NAMESPACE

class analysislist : public QDialog
{
    Q_OBJECT

public:
    analysislist(QWidget *parent = nullptr);
    ~analysislist();

private:
    Ui::analysislist *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void selectAll();
    void add();
    void del();
    void save();
    void search();
private slots:
    void on_btnBack_clicked();
};

#endif // ANALYSISLIST_H
