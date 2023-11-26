#include "reportpatient.h"
#include "ui_reportpatient.h"

ReportPatient::ReportPatient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportPatient)
{
    ui->setupUi(this);
}

ReportPatient::~ReportPatient()
{
    delete ui;
}
