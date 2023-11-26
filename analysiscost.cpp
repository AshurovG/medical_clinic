#include "analysiscost.h"
#include "ui_analysiscost.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <string>
#include <QDebug>
#include "patientmenu.h"

analysiscost::analysiscost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::analysiscost)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    connect(ui->btnSearch,SIGNAL(clicked(bool)),this, SLOT(search()));
    // Количеств столбцов
    ui->twAnalysis->setColumnCount(2);
    // Возможность прокрутки
    ui->twAnalysis->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twAnalysis->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twAnalysis->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twAnalysis->setHorizontalHeaderItem(0,new QTableWidgetItem("Название анализа"));
    ui->twAnalysis->setHorizontalHeaderItem(1,new QTableWidgetItem("Стоимость"));
//    ui->twAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twAnalysis->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twAnalysis->setSortingEnabled(true);
    ui->twAnalysis->sortByColumn(0);
    ui->btnBack->setFocusPolicy(Qt::NoFocus); //! применить ко всем формам!!!
    // Запрет на изменение ячеек таблицы при отображении
    ui->twAnalysis->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void analysiscost::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void analysiscost::selectAll()
{
    // Очистить содержимое компонента
    ui->twAnalysis->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select analysis_name, analysis_cost from analysis";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if( query.isActive())
        ui->twAnalysis->setRowCount( query.size());
    else
        ui->twAnalysis->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twAnalysis->setItem(i,0,new
QTableWidgetItem(query.value("analysis_name").toString()));
        ui->twAnalysis->setItem(i,1,new
QTableWidgetItem(query.value("analysis_cost").toString()));
        i++;
    }
}

void analysiscost::search()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "select analysis_name, analysis_cost from analysis where analysis_name LIKE '%' || ? || '%'";
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
    ui->twAnalysis->setRowCount(query.numRowsAffected());
    if (query.size() == 0) {
        QMessageBox::critical(this,"Ошибка поиска", "Такого анализа в списке нет");
        return;
    }
    int i = 0;
    while(query.next())
    {
        ui->twAnalysis->setItem(i,0,new
QTableWidgetItem(query.value("analysis_name").toString()));
        ui->twAnalysis->setItem(i,1,new
QTableWidgetItem(query.value("analysis_cost").toString()));
        i++;
    }
}

analysiscost::~analysiscost()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void analysiscost::on_btnBack_clicked()
{
    this->close();
    patientmenu *pmenu = new patientmenu(this);
    pmenu->show();
}
