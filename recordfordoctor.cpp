#include "recordfordoctor.h"
#include "ui_recordfordoctor.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QComboBox>
#include <QDebug>
#include "patientmenu.h"

recordfordoctor::recordfordoctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordfordoctor)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnSend,SIGNAL(clicked(bool)),this, SLOT(send()));
}

void recordfordoctor::showEvent(QShowEvent *event) {
    //! Третий запрос
    QSqlQuery query2(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr2 = "select fio, specialization from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query2.exec(sqlstr2) )
    {
        QMessageBox::critical(this,"Error", query2.lastError().text());
        return;
    }

    QComboBox *doctor_ptr = ui->cbDoctorFio;
    while(query2.next())
    {
        QString doctorFio = query2.value("fio").toString();
        QString doctorSpecialization = query2.value("specialization").toString();  // добавляем поле "age"
        QString comboText = doctorFio + "(" + doctorSpecialization + ")"; // создаем строку, которую нужно добавить в список
        doctor_ptr->addItem(comboText);
    }

}

void recordfordoctor::sendToPatient() {
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into patient(fio,phone_number,email,date_of_birth) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->lePatiientFio->text());
    query.bindValue(1,ui->lePhoneNumber->text());
    query.bindValue(2,ui->leEmail->text());
    query.bindValue(3,ui->deDateOfBirth->date());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }

}

void recordfordoctor::sendToRecording() {
    QSqlQuery query(dbconn);
//    // Создать строку запроса
//    QString sqlstr = "insert into recording_for_doctor(recording_date,recording_time,doctor_id,patient_id) values(?,?,?,?)";
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
        if (ui->lePatiientFio->text() == query.value("fio").toString()) {
            currentPatientId = query.value("patient_id").toInt();
        }
    }

    //! Второй запрос
    QSqlQuery query1(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr1 = "select doctor_id, fio from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query1.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query1.lastError().text());
        return;
    }
    QString str = ui->cbDoctorFio->currentText();
    std::string forCompare = str.mid(0, str.indexOf('(')).toStdString();
    int currentDoctorId;
    while(query1.next())
    {
        if (forCompare == query1.value("fio").toString().toStdString()) {
            currentDoctorId = query1.value("doctor_id").toInt();
        }
    }

    QSqlQuery queryR(dbconn);
    // Создать строку запроса
    QString sqlstrR = "insert into recording_for_doctor(recording_date, recording_time, doctor_id, patient_id) values(?,?,?,?)";
    // Подготовить запрос
    queryR.prepare(sqlstrR);
    // Передать параметры из полей ввода в запрос
    queryR.bindValue(0, QVariant::fromValue(ui->deRecordingDate->date()));
    queryR.bindValue(1, QVariant::fromValue(ui->teRecordingTime->time()));
    queryR.bindValue(2,currentDoctorId);
    qDebug() << currentDoctorId;
    queryR.bindValue(3,currentPatientId);
    // Выполнить запрос
    if( !queryR.exec() )
    {
        QMessageBox::critical(this,"Error",queryR.lastError().text());
        return;
    }
    QMessageBox::information(this,"Успешно", "Вы успешно запсались к врачу!");
}



void recordfordoctor::send()
{
    sendToPatient();
    sendToRecording();
}

recordfordoctor::~recordfordoctor()
{
    delete ui;
}

//void recordforsurg::on_btnBack_clicked()
//{
//    this->close();
//    doctormenu *dmenu = new doctormenu(this);
//    dmenu->show();
//}

void recordfordoctor::on_btnBack_clicked()
{
    this->close();
    patientmenu *pmenu = new patientmenu(this);
    pmenu->show();
}
