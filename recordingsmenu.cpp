#include "recordingsmenu.h"
#include "ui_recordingsmenu.h"
#include "recordingforanalysislist.h"
#include "recordingfordoctorlist.h"
#include "recordingforsurgery.h"
#include "recordingforwardlist.h"
#include "adminmenu.h"

recordingsmenu::recordingsmenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordingsmenu)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

recordingsmenu::~recordingsmenu()
{
    delete ui;
}

void recordingsmenu::on_btndoctor_clicked()
{
    this->close();
    recordingfordoctorlist *rfd = new recordingfordoctorlist(this);
    rfd->show();
}

void recordingsmenu::on_btnAnalysis_clicked()
{
    this->close();
    recordingforanalysislist *rfa = new recordingforanalysislist(this);
    rfa->show();
}

void recordingsmenu::on_btnSurgery_clicked()
{
    this->close();
    recordingforsurgery *rfs = new recordingforsurgery(this);
    rfs->show();
}

void recordingsmenu::on_btnWard_clicked()
{
    this->close();
    recordingforwardlist *rfw = new recordingforwardlist(this);
    rfw->show();
}

void recordingsmenu::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
