#include "recfordoc.h"
#include "ui_recfordoc.h"
#include "listfordoctor.h"
#include <QDebug>;
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QHeaderView>

recfordoc::recfordoc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recfordoc)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    ui->twRecPatients->setColumnCount(3);
    // Возможность прокрутки
    ui->twRecPatients->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twRecPatients->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twRecPatients->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы

    ui->twRecPatients->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО пациента"));
    ui->twRecPatients->setHorizontalHeaderItem(1,new QTableWidgetItem("Дата записи"));
    ui->twRecPatients->setHorizontalHeaderItem(2,new QTableWidgetItem("Время записи"));
    // Последний столбец растягивается при изменении размера формы

    ui->twRecPatients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Разрешаем сортировку пользователю
    ui->twRecPatients->setSortingEnabled(true);
    ui->twRecPatients->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twRecPatients->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void recfordoc::showEvent(QShowEvent *event) {
    extern int globalCurrentId;
    selectAll(globalCurrentId);
}

void  recfordoc::selectAll(int sendItem) {
    //Создаем запрос для подчиненной таблицы
    QSqlQuery query2(dbconn);
    query2.prepare("select patient.fio, recording_date, recording_time from recording_for_doctor join patient on patient.patient_id = recording_for_doctor.patient_id where recording_for_doctor.doctor_id = ? order by recording_date");
    query2.addBindValue(sendItem); //Передаем параметр из главной таблицы в запрос
    if( !query2.exec() )
    {
        QMessageBox::critical(this,"Error", query2.lastError().text());
        return;
    }


    if( query2.isActive())
    {
        ui->twRecPatients->setRowCount( query2.size());
    }
    else
        ui->twRecPatients->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query2.next())
    {
        qDebug() << "FKLDKFL";
        ui->twRecPatients->setItem(i,0,new
QTableWidgetItem(query2.value("patient.fio").toString()));
        ui->twRecPatients->setItem(i,1,new
QTableWidgetItem(query2.value("recording_date").toString()));
//        ui->twRecPatients->setItem(i,2,new
//QTableWidgetItem(query2.value("recording_time").toString()));
        QTableWidgetItem *item = new QTableWidgetItem(query2.value("recording_time").toTime().toString("H:mm"));
        ui->twRecPatients->setItem(i, 2, item);
        i++;
    }
}



recfordoc::~recfordoc()
{
    delete ui;
}

void recfordoc::on_btnBack_clicked()
{
    this->close();
    listfordoctor *ldoc = new listfordoctor(this);
    ldoc->show();
}
