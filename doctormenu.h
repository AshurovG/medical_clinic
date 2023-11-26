#ifndef DOCTORMENU_H
#define DOCTORMENU_H

#include <QDialog>

namespace Ui {
class doctormenu;
}

class doctormenu : public QDialog
{
    Q_OBJECT

public:
    explicit doctormenu(QWidget *parent = nullptr);
    ~doctormenu();

signals:
    void firstWindow();

private slots:
    void on_btnChangeUser_clicked();

    void on_btnForAnalysis_clicked();

    void on_btnForSurgery_clicked();

    void on_btnForWard_clicked();

    void on_btnRecordingList_clicked();

private:
    Ui::doctormenu *ui;
};

#endif // DOCTORMENU_H
