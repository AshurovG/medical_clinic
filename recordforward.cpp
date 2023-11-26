#include "recordforward.h"
#include "ui_recordforward.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QComboBox>
#include <QDebug>
#include "doctormenu.h"

recordforward::recordforward(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordforward)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
}

void recordforward::showEvent(QShowEvent *event) {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select fio from patient";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    QComboBox *patient_ptr = ui->cbPatient;
    while(query.next())
    {
        patient_ptr->addItem(query.value("fio").toString());
    }

    //! Второй запрос
    QSqlQuery query1(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr1 = "select ward_number, ward_branch from ward";
    // Выполнить запрос и поверить его успешность
    if( !query1.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query1.lastError().text());
        return;
    }

    QComboBox *ward_ptr = ui->cbWard;
    while(query1.next())
    {
        QString wardNumber = query1.value("ward_number").toString();
        QString wardBranch = query1.value("ward_branch").toString();  // добавляем поле "age"
        QString comboText = wardNumber + " (" + wardBranch + ")"; // создаем строку, которую нужно добавить в список
        ward_ptr->addItem(comboText);
    }

} //здесь можно сразу при открытии формы подключиться к БД

void recordforward::add()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select patient_id, fio from patient";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    int currentPatientId;
    while(query.next())
    {
        if (ui->cbPatient->currentText() == query.value("fio").toString()) {
            currentPatientId = query.value("patient_id").toInt();
        }
    }

    //! Второй запрос
    QSqlQuery query1(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr1 = "select ward_id, ward_number from ward";
    // Выполнить запрос и поверить его успешность
    if( !query1.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query1.lastError().text());
        return;
    }
    QString str = ui->cbWard->currentText();
    std::string forCompare = str.mid(0, str.indexOf(' ')).toStdString();
    int currentWardId;
    while(query1.next())
    {
        if (forCompare == query1.value("ward_number").toString().toStdString()) {
            currentWardId = query1.value("ward_id").toInt();
        }
    }

    QSqlQuery queryR(dbconn);
    // Создать строку запроса
    QString sqlstrR = "insert into recording_for_ward(patient_id, ward_id, first_date, last_date) values(?,?,?,?)";
    // Подготовить запрос
    queryR.prepare(sqlstrR);
    // Передать параметры из полей ввода в запрос
    queryR.bindValue(0,currentPatientId);
    queryR.bindValue(1,currentWardId);
    queryR.bindValue(2,ui->deFirstDate->text());
    queryR.bindValue(3,ui->deLastDate->text());
    // Выполнить запрос
    if( !queryR.exec() )
    {
        QMessageBox::critical(this,"Error",queryR.lastError().text());
        return;
    }
    QMessageBox::information(this,"Успешно", "Вы успешно записали пациента на данный анализ!");
}

recordforward::~recordforward()
{
    delete ui;
}

void recordforward::on_btnBack_clicked()
{
    this->close();
    doctormenu *dmenu = new doctormenu(this);
    dmenu->show();
}
