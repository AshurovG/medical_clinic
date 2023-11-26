#ifndef LISTFORDOCTOR_H
#define LISTFORDOCTOR_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class listfordoctor;
}

class listfordoctor : public QDialog
{
    Q_OBJECT

public:
    explicit listfordoctor(QWidget *parent = nullptr);
    ~listfordoctor();

private:
    Ui::listfordoctor *ui;
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
private slots:
//    void on_btnBack_clicked();
    void on_btnBack_3_clicked();
    void on_btnRecordings_clicked();
    int findId();
};

#endif // LISTFORDOCTOR_H
