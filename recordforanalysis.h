#ifndef RECORDFORANALYSIS_H
#define RECORDFORANALYSIS_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class recordforanalysis;
}

class recordforanalysis : public QDialog
{
    Q_OBJECT

public:
    explicit recordforanalysis(QWidget *parent = nullptr);
    ~recordforanalysis();

private slots:
    void add();
    void on_btnBack_clicked();

private:
    Ui::recordforanalysis *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // RECORDFORANALYSIS_H
