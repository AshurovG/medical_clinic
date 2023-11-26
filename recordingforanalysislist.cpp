#include "recordingforanalysislist.h"
#include "ui_recordingforanalysislist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include "recordingsmenu.h"


recordingforanalysislist::recordingforanalysislist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordingforanalysislist)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
//    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    // Количеств столбцов
    ui->twRecordingForAnalysis->setColumnCount(5);
    // Возможность прокрутки
    ui->twRecordingForAnalysis->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twRecordingForAnalysis->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twRecordingForAnalysis->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twRecordingForAnalysis->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО врача"));
    ui->twRecordingForAnalysis->setHorizontalHeaderItem(1,new QTableWidgetItem("ФИО пациента"));
    ui->twRecordingForAnalysis->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата записи"));
    ui->twRecordingForAnalysis->setHorizontalHeaderItem(3,new QTableWidgetItem("Время записи"));
    ui->twRecordingForAnalysis->setHorizontalHeaderItem(4,new QTableWidgetItem("Название анализа"));

//    ui->twRecordingForAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twRecordingForAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twRecordingForAnalysis->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twRecordingForAnalysis->setSortingEnabled(true);
    ui->twRecordingForAnalysis->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twRecordingForAnalysis->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void recordingforanalysislist::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void recordingforanalysislist::selectAll()
{
    // Очистить содержимое компонента
    ui->twRecordingForAnalysis->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "SELECT doctor.fio, patient.fio, analysis_date, analysis_time, analysis.analysis_name FROM recording_for_analysis JOIN doctor ON doctor.doctor_id = recording_for_analysis.doctor_id JOIN patient ON patient.patient_id = recording_for_analysis.patient_id JOIN analysis ON analysis.analysis_id = recording_for_analysis.analysis_id ORDER BY analysis.analysis_name";
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
        ui->twRecordingForAnalysis->setRowCount( query.size());
    else
        ui->twRecordingForAnalysis->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twRecordingForAnalysis->setItem(i,0,new
QTableWidgetItem(query.value("doctor.fio").toString()));
        ui->twRecordingForAnalysis->setItem(i,1,new
QTableWidgetItem(query.value("patient.fio").toString()));
        ui->twRecordingForAnalysis->setItem(i,2,new
QTableWidgetItem(query.value("analysis_date").toString()));
        QTableWidgetItem *item = new QTableWidgetItem(query.value("analysis_time").toTime().toString("H:mm"));
        ui->twRecordingForAnalysis->setItem(i, 3, item);
        ui->twRecordingForAnalysis->setItem(i,4,new
QTableWidgetItem(query.value("analysis.analysis_name").toString()));
        i++;
    }
}



recordingforanalysislist::~recordingforanalysislist()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void recordingforanalysislist::on_btnBack_clicked()
{
    this->close();
    recordingsmenu *rmenu = new recordingsmenu(this);
    rmenu->show();
}
