#ifndef WARDSLIST_H
#define WARDSLIST_H

#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class wardslist;
}

class wardslist : public QDialog
{
    Q_OBJECT

public:
    explicit wardslist(QWidget *parent = nullptr);
    ~wardslist();

private:
    Ui::wardslist *ui;
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
private slots:
    void on_btnBack_clicked();
};

#endif // WARDSLIST_H
