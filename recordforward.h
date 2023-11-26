#ifndef RECORDFORWARD_H
#define RECORDFORWARD_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class recordforward;
}

class recordforward : public QDialog
{
    Q_OBJECT

public:
    explicit recordforward(QWidget *parent = nullptr);
    ~recordforward();

private slots:
    void add();
    void on_btnBack_clicked();

private:
    Ui::recordforward *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RECORDFORWARD_H
