#include "doctorslist.h"
#include "ui_doctorslist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "adminmenu.h"

extern int rowIndex = 1;

doctorslist::doctorslist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doctorslist)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnPrevious,SIGNAL(clicked(bool)),this, SLOT(previous()));
    connect(ui->btnNext,SIGNAL(clicked(bool)),this, SLOT(next()));
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
}

void doctorslist::showEvent(QShowEvent *event) {

    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if(query.first()) { //!!! ЗДЕСЬ ПЕРЕДАЕМ ФИО ИЗ БД В ПОЛЕ lineEdit !!!
        QString fio = query.value("fio").toString();
        QString specialization = query.value("specialization").toString();
        QString cabinetNumber = query.value("cabinet").toString();
        QTime startTime = query.value("start_time").toTime();
        QTime endTime = query.value("end_time").toTime();
        QString cost = query.value("one_price").toString();
        ui->leFio_3->setText(fio);
        ui->leSpecialization_3->setText(specialization);
        ui->leCabinetNumber_3->setText(cabinetNumber);
        ui->teStartTime_3->setTime(startTime);
        ui->teEndTime_3->setTime(endTime);
        ui->leCost_3->setText(cost);
    }
} //здесь можно сразу при открытии формы подключиться к БД

void doctorslist::clearInf() {
    ui->leFio_3->clear();
    ui->leCabinetNumber_3->clear();
    ui->leCost_3->clear();
    ui->leSpecialization_3->clear();
    ui->teStartTime_3->clear();
    ui->teEndTime_3->clear();
}

int doctorslist::countRows() {
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

bool doctorslist::checkPrevious(int ind) {
    if (--ind <= 0) {
        return false;
    }
    return true;
}

bool doctorslist::checkNext(int ind) {
    if (++ind > countRows()) {
        return false;
    }
    return true;
}

void doctorslist::next()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if (checkNext(rowIndex)) {;
        rowIndex++;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex) {
                QString fio = query.value("fio").toString();
                QString specialization = query.value("specialization").toString();
                QString cabinetNumber = query.value("cabinet").toString();
                QTime startTime = query.value("start_time").toTime();
                QTime endTime = query.value("end_time").toTime();
                QString cost = query.value("one_price").toString();
                ui->leFio_3->setText(fio);
                ui->leSpecialization_3->setText(specialization);
                ui->leCabinetNumber_3->setText(cabinetNumber);
                ui->teStartTime_3->setTime(startTime);
                ui->teEndTime_3->setTime(endTime);
                ui->leCost_3->setText(cost);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Вы уже на последней записи списка!");
    }
}

void doctorslist::previous()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if (checkPrevious(rowIndex)) {
        rowIndex--;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex) {
                QString fio = query.value("fio").toString();
                QString specialization = query.value("specialization").toString();
                QString cabinetNumber = query.value("cabinet").toString();
                QTime startTime = query.value("start_time").toTime();
                QTime endTime = query.value("end_time").toTime();
                QString cost = query.value("one_price").toString();
                ui->leFio_3->setText(fio);
                ui->leSpecialization_3->setText(specialization);
                ui->leCabinetNumber_3->setText(cabinetNumber);
                ui->teStartTime_3->setTime(startTime);
                ui->teEndTime_3->setTime(endTime);
                ui->leCost_3->setText(cost);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Вы уже на первой записи списка!");
    }
}

void doctorslist::save() {
    if( QMessageBox::question(this,"Edit","Update?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QString fio_up = ui->leFio_3->text();
    QString specialization_up = ui->leSpecialization_3->text();
    QString cabinetNumber_up = ui->leCabinetNumber_3->text();
    QString startTime_up = ui->teStartTime_3->text();
    QString endTime_up = ui->teEndTime_3->text();
    QString cost_up = ui->leCost_3->text();

    QSqlQuery query(dbconn);
    QString sqlstr = "update doctor set fio='"+fio_up+"', specialization='"+specialization_up+"', cabinet='"+cabinetNumber_up+"', start_time='"+startTime_up+"', end_time='"+endTime_up+"', one_price='"+cost_up+"'  where fio = '"
            + ui->leFio_3->text() + "' or fio = ''";    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
}

void doctorslist::add()
{
    rowIndex = countRows() + 1;
    clearInf();
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into doctor(fio,specialization,cabinet,start_time,end_time,one_price) values(?,?,?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leFio_3->text());
    query.bindValue(1,ui->leSpecialization_3->text());
    query.bindValue(2,ui->leCabinetNumber_3->text().toInt());
    query.bindValue(3,ui->teStartTime_3->time());
    query.bindValue(4,ui->teEndTime_3->time());
    query.bindValue(5,ui->leCost_3->text().toDouble());
    // Если тип поля отличается от строкового, то преобразовать его
    //query.bindValue(3,ui->leInn->text().toLongLong()); !!!
    // Выполнить запрос
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
}

void doctorslist::del()
{
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Delete","Delete?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    //!!! Здесь удаляем по ФИО но можно исправить !!!
    QString sqlstr = "delete from doctor where fio = '"
            + ui->leFio_3->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    if (rowIndex > countRows()) {
        previous();
    } else {
        next();
    }
}



doctorslist::~doctorslist()
{
    delete ui;
}

void doctorslist::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
