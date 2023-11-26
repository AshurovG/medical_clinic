#include "surgeryandroom.h"
#include "ui_surgeryandroom.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QHeaderView>
#include "adminmenu.h"

extern int rowIndex2 = 1;

surgeryandroom::surgeryandroom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::surgeryandroom)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnPrevious,SIGNAL(clicked(bool)),this, SLOT(previous()));
    connect(ui->btnNext,SIGNAL(clicked(bool)),this, SLOT(next()));
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnSave,SIGNAL(clicked(bool)),this, SLOT(save()));
    connect(ui->btnAdd2,SIGNAL(clicked(bool)),this, SLOT(add2()));
    connect(ui->btnDel2,SIGNAL(clicked(bool)),this, SLOT(del2()));
    connect(ui->btnSave2,SIGNAL(clicked(bool)),this, SLOT(save2()));

    ui->twSurgery->setColumnCount(2);
    // Возможность прокрутки
    ui->twSurgery->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twSurgery->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twSurgery->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы

    ui->twSurgery->setHorizontalHeaderItem(0,new QTableWidgetItem("Название операции"));
    ui->twSurgery->setHorizontalHeaderItem(1,new QTableWidgetItem("Стоимость"));
    // Последний столбец растягивается при изменении размера формы
//    ui->twSurgery->horizontalHeader()->setStretchLastSection(true);
//    ui->twSurgery->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twSurgery->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twSurgery->horizontalHeader()->setStretchLastSection(true);


    // Разрешаем сортировку пользователю
    ui->twSurgery->setSortingEnabled(true);
    ui->twSurgery->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
//    ui->twSurgery->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void surgeryandroom::showEvent(QShowEvent *event) {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from operating_room";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if(query.first()) {
        // Привязка значения переменной к параметру запроса
        QString department = query.value("department").toString();
        QString operatingRoomNumber = query.value("operating_room_number").toString();
        ui->leDepartment->setText(department);
        ui->leOperatingRoomNumber->setText(operatingRoomNumber);
        selectAll(query.value("operating_room_id").toInt());
    }
}

int surgeryandroom::searchId() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from operating_room";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return -1;
    }

    int i=0;
    while(query.next())
    {
        if (query.value("department").toString() == ui->leDepartment->text())
        {
            if (query.value("operating_room_number").toString() == ui->leOperatingRoomNumber->text())
            {
                return query.value("operating_room_id").toInt();
                i++;
            }
        }
    }
    return -1;
}

void surgeryandroom::selectAll(int sendItem) {
    //Создаем запрос для подчиненной таблицы
    QSqlQuery query2(dbconn);
    query2.prepare("SELECT * FROM surgery WHERE operating_room_id = ?");
    query2.addBindValue(sendItem); //Передаем параметр из главной таблицы в запрос
    if( !query2.exec() )
    {
        QMessageBox::critical(this,"Error", query2.lastError().text());
        return;
    }


    if( query2.isActive())
    {
        ui->twSurgery->setRowCount( query2.size());
    }
    else
        ui->twSurgery->setRowCount( 0);
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query2.next())
    {
        ui->twSurgery->setItem(i,0,new
QTableWidgetItem(query2.value("surgery_name").toString()));
        ui->twSurgery->setItem(i,1,new
QTableWidgetItem(query2.value("surgery_cost").toString()));
        i++;
    }
}

void surgeryandroom::clearInf() {
    ui->leDepartment->clear();
    ui->leOperatingRoomNumber->clear();
}

int surgeryandroom::countRows() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from operating_room";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return 0;
    }

    int count = 0;
    while (query.next()) {
        count++;
    }
    return count;
}

bool surgeryandroom::checkPrevious(int ind) {
    if (--ind <= 0) {
        return false;
    }
    return true;
}

bool surgeryandroom::checkNext(int ind) {
    if (++ind > countRows()) {
        return false;
    }
    return true;
}

void surgeryandroom::next()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from operating_room";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if (checkNext(rowIndex2)) {
        rowIndex2++;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex2) {
                QString department = query.value("department").toString();
                QString operatingRoomNumber = query.value("operating_room_number").toString();
                ui->leDepartment->setText(department);
                ui->leOperatingRoomNumber->setText(operatingRoomNumber);
                ui->twSurgery->clearContents();

                //Создаем запрос для подчиненной таблицы
                selectAll(query.value("operating_room_id").toInt());
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Больше операционных в списке нет!");
    }
}

void surgeryandroom::previous()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from operating_room";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if (checkPrevious(rowIndex2)) {
        rowIndex2--;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex2) {
                QString department = query.value("department").toString();
                QString operatingRoomNumber= query.value("operating_room_number").toString();
                ui->leDepartment->setText(department);
                ui->leOperatingRoomNumber->setText(operatingRoomNumber);
                ui->twSurgery->clearContents();

                //Создаем запрос для подчиненной таблицы
                selectAll(query.value("operating_room_id").toInt());
            }
        }
    }
    else {
        QMessageBox::critical(this,"Ошибка", "Вы уже на первой записи списка!");
    }
}

void surgeryandroom::save() {
    if( QMessageBox::question(this,"Edit","Update?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QString department_up = ui->leDepartment->text();
    QString operatingRoomNumber_up = ui->leOperatingRoomNumber->text();

    QSqlQuery query(dbconn);
    QString sqlstr = "update operating_room set department='"+department_up+"', operating_room_number='"+operatingRoomNumber_up+"'  where operating_room_number = '"
            + ui->leOperatingRoomNumber->text() + "' or operating_room_number = 0";    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
}

void surgeryandroom::add()
{
    ui->twSurgery->clearContents();
    ui->twSurgery->setRowCount(0);
    rowIndex2 = countRows() + 1;
    clearInf();
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into operating_room(department,operating_room_number) values(?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leDepartment->text());
    query.bindValue(1,ui->leOperatingRoomNumber->text().toInt());
    // Выполнить запрос
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    ui->leOperatingRoomNumber->setText("0");
}

void surgeryandroom::del()
{
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Delete","Delete?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    QString sqlstr = "delete from operating_room where operating_room_number = '"
            + ui->leOperatingRoomNumber->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    if (rowIndex2 > countRows()) {
        previous();
    } else {
        next();
    }
}

void surgeryandroom::add2()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into surgery(surgery_name,surgery_cost, operating_room_id) values('', 0, ?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    int id = searchId();
    query.bindValue(0,id);
    // Выполнить запрос
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение о добавлении строки
    // и обновить записи в компоненте таблицы
    selectAll(id);
}

void surgeryandroom::del2() {
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twSurgery->currentRow();
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
    QString sqlstr = "delete from surgery where surgery_name = '"
            + ui->twSurgery->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    selectAll(searchId());
}

void surgeryandroom::save2() {
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twSurgery->currentRow();
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
    QTableWidgetItem *item = ui->twSurgery->item(currow, 0); // получаем указатель на объект ячейки
    QString surgeryName_up = item->text();
    qDebug() << surgeryName_up;
    item = ui->twSurgery->item(currow, 1); // получаем указатель на объект ячейки
    QString surgeryCost_up = item->text();
    qDebug() << surgeryCost_up;

    QSqlQuery query(dbconn);
    QString sqlstr = "update surgery set surgery_name='"+surgeryName_up+"', surgery_cost='"+surgeryCost_up+"'  where surgery_name = '"
            + ui->twSurgery->item(currow,0)->text() + "' or surgery_name = ''";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    selectAll(searchId());
}


surgeryandroom::~surgeryandroom()
{
    delete ui;
}

void surgeryandroom::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
