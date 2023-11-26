#include "analysislist.h"
#include "ui_analysislist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <string>
#include <QDebug>
#include "adminmenu.h"

analysislist::analysislist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::analysislist)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    connect(ui->btnSearch,SIGNAL(clicked(bool)),this, SLOT(search()));
    // Количеств столбцов
    ui->twAnalysis->setColumnCount(3);
    // Возможность прокрутки
    ui->twAnalysis->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twAnalysis->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twAnalysis->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twAnalysis->setHorizontalHeaderItem(0,new QTableWidgetItem("Название анализа"));
    ui->twAnalysis->setHorizontalHeaderItem(1,new QTableWidgetItem("Кабинет"));
    ui->twAnalysis->setHorizontalHeaderItem(2,new QTableWidgetItem("Стоимость"));
//    ui->twAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twAnalysis->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twAnalysis->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twAnalysis->setSortingEnabled(true);
    ui->twAnalysis->sortByColumn(0);
    ui->btnAdd->setFocusPolicy(Qt::NoFocus);
    ui->btnDel->setFocusPolicy(Qt::NoFocus);
    ui->btnSave->setFocusPolicy(Qt::NoFocus);
    ui->btnBack->setFocusPolicy(Qt::NoFocus); //! применить ко всем формам!!!
}

void analysislist::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД

void analysislist::selectAll()
{
    // Очистить содержимое компонента
    ui->twAnalysis->clearContents();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from analysis";
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
QTableWidgetItem(query.value("cabinet").toString()));
        ui->twAnalysis->setItem(i,2,new
QTableWidgetItem(query.value("analysis_cost").toString()));
        i++;
    }
}

void analysislist::search()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "select * from analysis where analysis_name LIKE '%' || ? || '%'";
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
QTableWidgetItem(query.value("cabinet").toString()));
        ui->twAnalysis->setItem(i,2,new
QTableWidgetItem(query.value("analysis_cost").toString()));
        i++;
    }
}

void analysislist::add()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into analysis(analysis_name,cabinet, analysis_cost) values('', 0, 0)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Выполнить запрос
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение о добавлении строки
    //ТЕСТ
    // и обновить записи в компоненте таблицы
    int row = ui->twAnalysis->currentRow();
    QTableWidgetItem* item = new QTableWidgetItem("");
    ui->twAnalysis->setItem(row, 0, item);

    selectAll();
}

void analysislist::del()
{
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twAnalysis->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Delete","Delete row?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    QString sqlstr = "delete from analysis where analysis_name = '"
            + ui->twAnalysis->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    //ТЕСТ
    // Обновить содержимое компонента таблицы
    selectAll();
}

void analysislist::save() {
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twAnalysis->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    if( QMessageBox::question(this,"Edit","Update row?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QTableWidgetItem *item = ui->twAnalysis->item(currow, 0); // получаем указатель на объект ячейки
    QString analysisName_up = item->text();
    item = ui->twAnalysis->item(currow, 1); // получаем указатель на объект ячейки
    QString cabinet_up = item->text();
    item = ui->twAnalysis->item(currow, 2            );
    QString analysisCost_up = item->text();

    QSqlQuery query(dbconn);
    QString sqlstr = "update analysis set analysis_name='"+analysisName_up+"', cabinet='"+cabinet_up+"', analysis_cost='"+analysisCost_up+"'  where analysis_name = '"
            + ui->twAnalysis->item(currow,0)->text() + "' or analysis_name = ''";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    //ТЕСТ
    // Обновить содержимое компонента таблицы
    selectAll();
}


analysislist::~analysislist()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}




void analysislist::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
