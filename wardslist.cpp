#include "wardslist.h"
#include "ui_wardslist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "adminmenu.h"

extern int rowIndex1 = 1;

wardslist::wardslist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wardslist)
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

void wardslist::showEvent(QShowEvent *event) {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from ward";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if(query.first()) { //!!! ЗДЕСЬ ПЕРЕДАЕМ ФИО ИЗ БД В ПОЛЕ lineEdit !!!
        QString wardNumber = query.value("ward_number").toString();
        QString wardBranch = query.value("ward_branch").toString();
        QString capacity = query.value("capacity").toString();
        ui->leWardNumber->setText(wardNumber);
        ui->leWardBranch->setText(wardBranch);
        ui->leCapacity->setText(capacity);
    }
} //здесь можно сразу при открытии формы подключиться к БД

void wardslist::clearInf() {
    ui->leWardNumber->clear();
    ui->leWardBranch->clear();
    ui->leCapacity->clear();
}

int wardslist::countRows() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from ward";
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

bool wardslist::checkPrevious(int ind) {
    if (--ind <= 0) {
        return false;
    }
    return true;
}

bool wardslist::checkNext(int ind) {
    if (++ind > countRows()) {
        return false;
    }
    return true;
}

void wardslist::next()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from ward";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if (checkNext(rowIndex1)) {
        rowIndex1++;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex1) {
                QString wardNumber = query.value("ward_number").toString();
                QString wardBranch = query.value("ward_branch").toString();
                QString capacity = query.value("capacity").toString();
                ui->leWardNumber->setText(wardNumber);
                ui->leWardBranch->setText(wardBranch);
                ui->leCapacity->setText(capacity);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Больше палат в списке нет!");
    }
}

void wardslist::previous()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from ward";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if (checkPrevious(rowIndex1)) {
        rowIndex1--;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex1) {
                QString wardNumber = query.value("ward_number").toString();
                QString wardBranch = query.value("ward_branch").toString();
                QString capacity = query.value("capacity").toString();
                ui->leWardNumber->setText(wardNumber);
                ui->leWardBranch->setText(wardBranch);
                ui->leCapacity->setText(capacity);
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Вы уже на первой записи списка!");
    }
}

void wardslist::save() {
    if( QMessageBox::question(this,"Edit","Update?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QString wardNumber_up = ui->leWardNumber->text();
    QString wardBranch_up = ui->leWardBranch->text();
    QString capacity_up = ui->leCapacity->text();

    QSqlQuery query(dbconn);
    QString sqlstr = "update ward set ward_number='"+wardNumber_up+"', ward_branch='"+wardBranch_up+"', capacity='"+capacity_up+"'  where ward_number = '"
            + ui->leWardNumber->text() + "' or ward_number = 0";    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
}

void wardslist::add()
{
    rowIndex1 = countRows() + 1;
    clearInf();
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into ward(ward_number,ward_branch,capacity) values(0,'',0)";
    // Подготовить запрос
    query.prepare(sqlstr);
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    ui->leWardNumber->setText("0");
    ui->leCapacity->setText("0");
}

void wardslist::del()
{
    if( QMessageBox::question(this,"Delete","Delete?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    QString sqlstr = "delete from ward where ward_number = '"
            + ui->leWardNumber->text() + "' OR ward_number=0";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    if (rowIndex1 > countRows()) {
        previous();
    } else {
        next();
    }
}

wardslist::~wardslist()
{
    delete ui;
}

void wardslist::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
