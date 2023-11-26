#include "surgerycost.h"
#include "ui_surgerycost.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <string>
#include <QDebug>
#include "patientmenu.h"

surgerycost::surgerycost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::surgerycost)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnSearch,SIGNAL(clicked(bool)),this, SLOT(search()));
    // Количеств столбцов
    ui->twSurgery->setColumnCount(2);
    // Возможность прокрутки
    ui->twSurgery->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twSurgery->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twSurgery->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twSurgery->setHorizontalHeaderItem(0,new QTableWidgetItem("Название операции"));
    ui->twSurgery->setHorizontalHeaderItem(1,new QTableWidgetItem("Стоимость"));
//    ui->twSurgery->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twSurgery->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twSurgery->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twSurgery->setSortingEnabled(true);
    ui->twSurgery->sortByColumn(0);
    ui->btnBack->setFocusPolicy(Qt::NoFocus); //! применить ко всем формам!!!
    // Запрет на изменение ячеек таблицы при отображении
    ui->twSurgery->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void surgerycost::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void surgerycost::selectAll()
{
    // Очистить содержимое компонента
    ui->twSurgery->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select surgery_name, surgery_cost from surgery";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if( query.isActive())
        ui->twSurgery->setRowCount( query.size());
    else
        ui->twSurgery->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twSurgery->setItem(i,0,new
QTableWidgetItem(query.value("surgery_name").toString()));
        ui->twSurgery->setItem(i,1,new
QTableWidgetItem(query.value("surgery_cost").toString()));
        i++;
    }
}

void surgerycost::search()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "select surgery_name, surgery_cost from surgery where surgery_name LIKE '%' || ? || '%'";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leSearchAnalysis->text());
    qDebug() << ui->leSearchAnalysis->text();
    // Выполнить запрос
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    ui->twSurgery->setRowCount(query.numRowsAffected());
    if (query.size() == 0) {
        QMessageBox::critical(this,"Ошибка поиска", "Такой операции в списке нет");
        return;
    }
    int i = 0;
    while(query.next())
    {
        ui->twSurgery->setItem(i,0,new
QTableWidgetItem(query.value("surgery_name").toString()));
        ui->twSurgery->setItem(i,1,new
QTableWidgetItem(query.value("surgery_cost").toString()));
        i++;
    }
}

surgerycost::~surgerycost()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void surgerycost::on_btnBack_clicked()
{
    this->close();
    patientmenu *pmenu = new patientmenu(this);
    pmenu->show();
}
