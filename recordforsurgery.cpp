#include "recordforsurgery.h"
#include "ui_recordforsurgery.h"

recordforsurgery::recordforsurgery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordforsurgery)
{
    ui->setupUi(this);
}

recordforsurgery::~recordforsurgery()
{
    delete ui;
}
