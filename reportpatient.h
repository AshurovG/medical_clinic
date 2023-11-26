#ifndef REPORTPATIENT_H
#define REPORTPATIENT_H

#include <QDialog>

namespace Ui {
class ReportPatient;
}

class ReportPatient : public QDialog
{
    Q_OBJECT

public:
    explicit ReportPatient(QWidget *parent = nullptr);
    ~ReportPatient();

private:
    Ui::ReportPatient *ui;
};

#endif // REPORTPATIENT_H
