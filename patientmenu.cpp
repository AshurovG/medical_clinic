#include "patientmenu.h"
#include "ui_patientmenu.h"
#include "authorization.h"
#include "recordfordoctor.h"
#include "analysiscost.h"
#include "surgerycost.h"

patientmenu::patientmenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::patientmenu)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

patientmenu::~patientmenu()
{
    delete ui;
}

void patientmenu::on_btnChangeUser_clicked()
{
    this->close();
    authorization *srl = new authorization(this);
    srl->show();
}

void patientmenu::on_btnAddRecording_clicked()
{
    this->close();
    recordfordoctor *rfd = new recordfordoctor(this);
    rfd->show();
}

void patientmenu::on_btnAnalysisCosts_clicked()
{
    this->close();
    analysiscost *ac = new analysiscost(this);
    ac->show();
}

void patientmenu::on_btnSurgeriesCosts_clicked()
{
    this->close();
    surgerycost *sc = new surgerycost(this);
    sc->show();
}
