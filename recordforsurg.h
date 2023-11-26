#ifndef RECORDFORSURG_H
#define RECORDFORSURG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class recordforsurg;
}

class recordforsurg : public QDialog
{
    Q_OBJECT

public:
    explicit recordforsurg(QWidget *parent = nullptr);
    ~recordforsurg();

private slots:
    void add();
    void on_btnBack_clicked();

private:
    Ui::recordforsurg *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RECORDFORSURG_H
