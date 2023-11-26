
#include "recordingfordoctorlist.h"
#include "ui_recordingfordoctorlist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "recordingsmenu.h"

recordingfordoctorlist::recordingfordoctorlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordingfordoctorlist)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
//    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    // Количеств столбцов
    ui->twRecordingForDoctor->setColumnCount(4);
    // Возможность прокрутки
    ui->twRecordingForDoctor->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twRecordingForDoctor->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twRecordingForDoctor->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twRecordingForDoctor->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО врача"));
    ui->twRecordingForDoctor->setHorizontalHeaderItem(1,new QTableWidgetItem("ФИО пациента"));
    ui->twRecordingForDoctor->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата записи"));
    ui->twRecordingForDoctor->setHorizontalHeaderItem(3,new QTableWidgetItem("Время записи"));

    ui->twRecordingForDoctor->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twRecordingForDoctor->horizontalHeader()->setStretchLastSection(true);
 // Разрешаем сортировку пользователю
    ui->twRecordingForDoctor->setSortingEnabled(true);
    ui->twRecordingForDoctor->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twRecordingForDoctor->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void recordingfordoctorlist::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void recordingfordoctorlist::selectAll()
{
    // Очистить содержимое компонента
    ui->twRecordingForDoctor->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT doctor.fio, patient.fio, recording_date, recording_time FROM recording_for_doctor JOIN doctor ON doctor.doctor_id = recording_for_doctor.doctor_id JOIN patient ON patient.patient_id = recording_for_doctor.patient_id ORDER BY doctor.fio";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
        ui->twRecordingForDoctor->setRowCount( query.size());
    else
        ui->twRecordingForDoctor->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twRecordingForDoctor->setItem(i,0,new
QTableWidgetItem(query.value("doctor.fio").toString()));
        ui->twRecordingForDoctor->setItem(i,1,new
QTableWidgetItem(query.value("patient.fio").toString()));
        ui->twRecordingForDoctor->setItem(i,2,new
QTableWidgetItem(query.value("recording_date").toString()));
//        ui->twRecordingForDoctor->setItem(i,3,new
//QTableWidgetItem(query.value("recording_time").toString()));
        QTableWidgetItem *item = new QTableWidgetItem(query.value("recording_time").toTime().toString("H:mm"));
        ui->twRecordingForDoctor->setItem(i, 3, item);
        i++;
        qDebug() << query.value("recording_time").toString();
    }
}


recordingfordoctorlist::~recordingfordoctorlist()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void recordingfordoctorlist::on_btnBack_clicked()
{
    this->close();
    recordingsmenu *rmenu = new recordingsmenu(this);
    rmenu->show();
}
