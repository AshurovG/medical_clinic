#include "listfordoctor.h"
#include "ui_listfordoctor.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QHeaderView>
#include "doctormenu.h"
#include "recfordoc.h"

extern int rowIndex3 = 1;

listfordoctor::listfordoctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listfordoctor)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnPrevious_3,SIGNAL(clicked(bool)),this, SLOT(previous()));
    connect(ui->btnNext_3,SIGNAL(clicked(bool)),this, SLOT(next()));
}

void listfordoctor::showEvent(QShowEvent *event) {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select fio, specialization, doctor_id from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if(query.first()) {
        // Привязка значения переменной к параметру запроса
        QString fio = query.value("fio").toString();
        QString specialization = query.value("specialization").toString();
        ui->leFio->setText(fio);
        ui->leSpecialization->setText(specialization);
    }





} //здесь можно сразу при открытии формы подключиться к БД

void listfordoctor::clearInf() {
    ui->leFio->clear();
    ui->leSpecialization->clear();
}

int listfordoctor::countRows() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return 0;
    }

    int count = 0;
    while (query.next()) {
        count++;
    }
    return count;
}

bool listfordoctor::checkPrevious(int ind) {
    if (--ind <= 0) {
        return false;
    }
    return true;
}

bool listfordoctor::checkNext(int ind) {
    if (++ind > countRows()) {
        return false;
    }
    return true;
}

int listfordoctor::findId() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select doctor_id, fio from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return 1;
    }
    int i = 0;
    while (query.next()) {
        i++;
        if (ui->leFio->text() == query.value("fio").toString()) {
            return query.value("doctor_id").toInt();
            }
    }
    return -1;
}

void listfordoctor::next()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select doctor_id, fio, specialization from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if (checkNext(rowIndex3)) {
        rowIndex3++;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex3) {
                QString fio = query.value("fio").toString();
                QString specialization = query.value("specialization").toString();
                ui->leFio->setText(fio);
                ui->leSpecialization->setText(specialization);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Больше врачей в списке нет!");
    }
}

void listfordoctor::previous()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select doctor_id, fio, specialization from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if (checkPrevious(rowIndex3)) {
        rowIndex3--;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex3) {
                QString fio = query.value("fio").toString();
                QString specialization= query.value("specialization").toString();
                ui->leFio->setText(fio);
                ui->leSpecialization->setText(specialization);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Вы уже на первой записи списка!");
    }
}



listfordoctor::~listfordoctor()
{
    delete ui;
}

void listfordoctor::on_btnBack_3_clicked()
{
    this->close();
    doctormenu *dmenu = new doctormenu(this);
    dmenu->show();
}
int globalCurrentId;

void listfordoctor::on_btnRecordings_clicked()
{
    globalCurrentId = findId();
    this->close();
    recfordoc *rfd = new recfordoc(this);
    rfd->show();
}
