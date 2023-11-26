#ifndef RECFORDOC_H
#define RECFORDOC_H
#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class recfordoc;
}

class recfordoc : public QDialog
{
    Q_OBJECT

public:
    explicit recfordoc(QWidget *parent = nullptr);
    ~recfordoc();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_btnBack_clicked();
    void selectAll(int sendItem);

private:
    Ui::recfordoc *ui;
    QSqlDatabase dbconn;
};

#endif // RECFORDOC_H
