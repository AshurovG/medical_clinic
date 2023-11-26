#ifndef RECORDINGSMENU_H
#define RECORDINGSMENU_H

#include <QDialog>

namespace Ui {
class recordingsmenu;
}

class recordingsmenu : public QDialog
{
    Q_OBJECT

public:
    explicit recordingsmenu(QWidget *parent = nullptr);
    ~recordingsmenu();

private slots:
    void on_btndoctor_clicked();

    void on_btnAnalysis_clicked();

    void on_btnSurgery_clicked();

    void on_btnWard_clicked();

    void on_btnBack_clicked();

private:
    Ui::recordingsmenu *ui;
};

#endif // RECORDINGSMENU_H
