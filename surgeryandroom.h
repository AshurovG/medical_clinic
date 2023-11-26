#ifndef SURGERYANDROOM_H
#define SURGERYANDROOM_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class surgeryandroom;
}

class surgeryandroom : public QDialog
{
    Q_OBJECT

public:
    explicit surgeryandroom(QWidget *parent = nullptr);
    ~surgeryandroom();

private:
    Ui::surgeryandroom *ui;
    QSqlDatabase dbconn;

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void clearInf();
    bool checkNext(int ind);
    bool checkPrevious(int ind);
    int countRows();
    void previous();
    void next();
    void add();
    void del();
    void save();
    void add2();
    int searchId();
    void del2();
    void save2();
    void selectAll(int sendItem);
private slots:
    void on_btnBack_clicked();
};

#endif // SURGERYANDROOM_H
