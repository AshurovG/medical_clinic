#include "doctormenu.h"
#include "ui_doctormenu.h"
#include "authorization.h"
#include "recordforanalysis.h"
#include "recordforsurg.h"
#include "recordforward.h"
#include "listfordoctor.h"

doctormenu::doctormenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doctormenu)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

doctormenu::~doctormenu()
{
    delete ui;
}

void doctormenu::on_btnChangeUser_clicked()
{
    this->close();
    authorization *srl = new authorization(this);
    srl->show();
}

void doctormenu::on_btnForAnalysis_clicked()
{
    this->close();
    recordforanalysis *rfa = new recordforanalysis(this);
    rfa->show();
}

void doctormenu::on_btnForSurgery_clicked()
{
    this->close();
    recordforsurg *rfs = new recordforsurg(this);
    rfs->show();
}

void doctormenu::on_btnForWard_clicked()
{
    this->close();
    recordforward *rfw = new recordforward(this);
    rfw->show();
}



void doctormenu::on_btnRecordingList_clicked()
{
    this->close();
    listfordoctor *listfd = new listfordoctor(this);
    listfd->show();
}
