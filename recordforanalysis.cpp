#include "recordforanalysis.h"
#include "ui_recordforanalysis.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QComboBox>
#include <QDebug>
#include "doctormenu.h"

recordforanalysis::recordforanalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordforanalysis)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
}

void recordforanalysis::showEvent(QShowEvent *event) {
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
    QString sqlstr1 = "select fio from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query1.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query1.lastError().text());
        return;
    }

    QComboBox *doctor_ptr = ui->cbDoctor;
    while(query1.next())
    {
        doctor_ptr->addItem(query1.value("fio").toString());
    }

    //! Третий запрос
    QSqlQuery query2(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr2 = "select analysis_name from analysis";
    // Выполнить запрос и поверить его успешность
    if( !query2.exec(sqlstr2) )
    {
        QMessageBox::critical(this,"Error", query2.lastError().text());
        return;
    }

    QComboBox *analysis_ptr = ui->cbAnalysis;
    while(query2.next())
    {
        analysis_ptr->addItem(query2.value("analysis_name").toString());
    }

} //здесь можно сразу при открытии формы подключиться к БД


void recordforanalysis::add()
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
    QString sqlstr1 = "select doctor_id, fio from doctor";
    // Выполнить запрос и поверить его успешность
    if( !query1.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query1.lastError().text());
        return;
    }

    int currentDoctorId;
    while(query1.next())
    {
        if (ui->cbDoctor->currentText() == query1.value("fio").toString()) {
            currentDoctorId = query1.value("doctor_id").toInt();
        }
    }

    //! Третий запрос
    QSqlQuery query2(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr2 = "select analysis_id, analysis_name from analysis";
    // Выполнить запрос и поверить его успешность
    if( !query2.exec(sqlstr2) )
    {
        QMessageBox::critical(this,"Error", query2.lastError().text());
        return;
    }

    int currentAnalysisId;
    while(query2.next())
    {
        if (ui->cbAnalysis->currentText() == query2.value("analysis_name").toString()) {
            currentAnalysisId = query2.value("analysis_id").toInt();
        }
    }

    QSqlQuery queryR(dbconn);
    // Создать строку запроса
    QString sqlstrR = "insert into recording_for_analysis(doctor_id,patient_id,analysis_date, analysis_time, analysis_id) values(?,?,?,?,?)";
    // Подготовить запрос
    queryR.prepare(sqlstrR);
    // Передать параметры из полей ввода в запрос
    queryR.bindValue(0,currentDoctorId);
    queryR.bindValue(1,currentPatientId);
    queryR.bindValue(2,ui->deDate->text());
    queryR.bindValue(3,ui->teTime->text());
    queryR.bindValue(4,currentAnalysisId);
    // Выполнить запрос
    if( !queryR.exec() )
    {
        QMessageBox::critical(this,"Error",queryR.lastError().text());
        return;
    }
    QMessageBox::information(this,"Успешно", "Вы успешно запсали пациента на анализ!");
}

recordforanalysis::~recordforanalysis()
{
    delete ui;
}

void recordforanalysis::on_btnBack_clicked()
{
    this->close();
    doctormenu *dmenu = new doctormenu(this);
    dmenu->show();
}
