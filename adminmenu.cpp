#include "adminmenu.h"
#include "ui_adminmenu.h"
#include "patientlist.h"
#include "doctorslist.h"
#include "surgeryandroom.h"
#include "wardslist.h"
#include "analysislist.h"
#include "authorization.h"
#include "recordingsmenu.h"

adminmenu::adminmenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminmenu)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

adminmenu::~adminmenu()
{
    delete ui;
}

void adminmenu::on_btnPatientList_clicked()
{
    this->close();
    patientList *patientlist = new patientList(this);
    patientlist->show();
}

void adminmenu::on_btnDoctorList_clicked()
{
    this->close();
    doctorslist *doctorl = new doctorslist(this);
    doctorl->show();
}

void adminmenu::on_btnWardList_clicked()
{
    this->close();
    wardslist *wardl = new wardslist(this);
    wardl->show();
}

void adminmenu::on_btnAnalysisList_clicked()
{
    this->close();
    analysislist *analysisl = new analysislist(this);
    analysisl->show();
}

void adminmenu::on_btnSurgeryAndOperatingRoom_clicked()
{
    this->close();
    surgeryandroom *srl = new surgeryandroom(this);
    srl->show();
}

void adminmenu::on_btnChangeUser_clicked()
{
    this->close();
    authorization *srl = new authorization(this);
    srl->show();
}

void adminmenu::on_btnAllRecording_clicked()
{
    this->close();
    recordingsmenu *rmenu = new recordingsmenu(this);
    rmenu->show();
}
