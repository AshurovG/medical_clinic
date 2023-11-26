#ifndef DOCTOR_H
#define DOCTOR_H
#include <QMainWindow>
#include <QSqlDatabase>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase dbconn;
public slots:
    void dbconnect();
    void selectAll();
    void add();
    void del();
    void update();
};
#endif // DOCTOR_H
