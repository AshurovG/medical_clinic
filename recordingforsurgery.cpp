
#include "recordingforsurgery.h"
#include "ui_recordingforsurgery.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "recordingsmenu.h"

recordingforsurgery::recordingforsurgery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordingforsurgery)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
//    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    // Количеств столбцов
    ui->twRecordingForSurgery->setColumnCount(5);
    // Возможность прокрутки
    ui->twRecordingForSurgery->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twRecordingForSurgery->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twRecordingForSurgery->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twRecordingForSurgery->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО назначившего врача"));
    ui->twRecordingForSurgery->setHorizontalHeaderItem(1,new QTableWidgetItem("ФИО пациента"));
    ui->twRecordingForSurgery->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата записи"));
    ui->twRecordingForSurgery->setHorizontalHeaderItem(3,new QTableWidgetItem("Время записи"));
    ui->twRecordingForSurgery->setHorizontalHeaderItem(4,new QTableWidgetItem("Название операции"));

    ui->twRecordingForSurgery->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twRecordingForSurgery->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twRecordingForSurgery->setSortingEnabled(true);
    ui->twRecordingForSurgery->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twRecordingForSurgery->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void recordingforsurgery::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД


void recordingforsurgery::selectAll()
{
    // Очистить содержимое компонента
    ui->twRecordingForSurgery->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    // выйти из функции
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT doctor.fio, patient.fio, recording_date, recording_time, surgery.surgery_name FROM recording_for_surgery JOIN doctor ON doctor.doctor_id = recording_for_surgery.doctor_id JOIN patient ON patient.patient_id = recording_for_surgery.patient_id JOIN surgery ON surgery.surgery_id = recording_for_surgery.surgery_id ORDER BY doctor.fio";
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
        ui->twRecordingForSurgery->setRowCount( query.size());
    else
        ui->twRecordingForSurgery->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twRecordingForSurgery->setItem(i,0,new
QTableWidgetItem(query.value("doctor.fio").toString()));
        ui->twRecordingForSurgery->setItem(i,1,new
QTableWidgetItem(query.value("patient.fio").toString()));
        ui->twRecordingForSurgery->setItem(i,2,new
QTableWidgetItem(query.value("recording_date").toString()));
        QTableWidgetItem *item = new QTableWidgetItem(query.value("recording_time").toTime().toString("H:mm"));
        ui->twRecordingForSurgery->setItem(i, 3, item);
        ui->twRecordingForSurgery->setItem(i,4,new
QTableWidgetItem(query.value("surgery.surgery_name").toString()));
        i++;
        qDebug() << query.value("recording_time").toString();
    }
}



recordingforsurgery::~recordingforsurgery()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void recordingforsurgery::on_btnBack_clicked()
{
    this->close();
    recordingsmenu *rmenu = new recordingsmenu(this);
    rmenu->show();
}
