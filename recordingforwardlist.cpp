
#include "recordingforwardlist.h"
#include "ui_recordingforwardlist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "recordingsmenu.h"

recordingforwardlist::recordingforwardlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordingforwardlist)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
//    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    // Количеств столбцов
    ui->twRecordingForWard->setColumnCount(4);
    // Возможность прокрутки
    ui->twRecordingForWard->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twRecordingForWard->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twRecordingForWard->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twRecordingForWard->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО пациента"));
    ui->twRecordingForWard->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер палаты"));
    ui->twRecordingForWard->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата заселения"));
    ui->twRecordingForWard->setHorizontalHeaderItem(3,new QTableWidgetItem("Дата выписки"));

    ui->twRecordingForWard->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twRecordingForWard->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twRecordingForWard->setSortingEnabled(true);
    ui->twRecordingForWard->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twRecordingForWard->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void recordingforwardlist::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void recordingforwardlist::selectAll()
{
    // Очистить содержимое компонента
    ui->twRecordingForWard->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT patient.fio, ward.ward_number, first_date, last_date FROM recording_for_ward JOIN patient ON patient.patient_id = recording_for_ward.patient_id JOIN ward ON ward.ward_id = recording_for_ward.ward_id ORDER BY first_date";
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
        ui->twRecordingForWard->setRowCount( query.size());
    else
        ui->twRecordingForWard->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twRecordingForWard->setItem(i,0,new
QTableWidgetItem(query.value("patient.fio").toString()));
        ui->twRecordingForWard->setItem(i,1,new
QTableWidgetItem(query.value("ward.ward_number").toString()));
        ui->twRecordingForWard->setItem(i,2,new
QTableWidgetItem(query.value("first_date").toString()));
        ui->twRecordingForWard->setItem(i,3,new
QTableWidgetItem(query.value("last_date").toString()));
        i++;
    }
}




recordingforwardlist::~recordingforwardlist()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void recordingforwardlist::on_btnBack_clicked()
{
    this->close();
    recordingsmenu *rmenu = new recordingsmenu(this);
    rmenu->show();
}
