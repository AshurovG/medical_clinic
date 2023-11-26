#ifndef DOCTORSLIST_H
#define DOCTORSLIST_H

#include <QDialog>
#include <QSqlDatabase>


namespace Ui {
class doctorslist;
}

class doctorslist : public QDialog
{
    Q_OBJECT

public:
    explicit doctorslist(QWidget *parent = nullptr);
    ~doctorslist();

private:
    Ui::doctorslist *ui;
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

#endif // DOCTORSLIST_H
