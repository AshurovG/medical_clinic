#include "patientlist.h"
#include "ui_patientlist.h"
#include <QDebug>

#include "patientlist.h"
#include "ui_patientlist.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include "adminmenu.h"

patientList::patientList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::patientList)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this, SLOT(add()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this, SLOT(del()));
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this, SLOT(update()));
    connect(ui->twPatient, &QTableWidget::cellClicked, this, &patientList::selectedRow);
    // Количеств столбцов
    ui->twPatient->setColumnCount(4);//тут изменил с 5 на 4
    // Возможность прокрутки
    ui->twPatient->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twPatient->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twPatient->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы

//    ui->twPatient->setHorizontalHeaderItem(0,new QTableWidgetItem("ID"));
    ui->twPatient->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twPatient->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер телефона"));
    ui->twPatient->setHorizontalHeaderItem(2,new QTableWidgetItem("Почта"));
    ui->twPatient->setHorizontalHeaderItem(3,new QTableWidgetItem("Дата рождения"));
//     ui->twPatient->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twPatient->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->twPatient->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twPatient->setSortingEnabled(true);
    ui->twPatient->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twPatient->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void patientList::selectedRow() {
    int currow = ui->twPatient->currentRow();
    ui->leFio->setText(ui->twPatient->item(currow,0)->text());
    ui->lePhoneNumber->setText(ui->twPatient->item(currow,1)->text());
    ui->leEmail->setText(ui->twPatient->item(currow,2)->text());
    QDate date = QDate::fromString(ui->twPatient->item(currow, 3)->text(), "yyyy-MM-dd");
    ui->deDateOfBirth->setDate(date);
}

void patientList::showEvent(QShowEvent *event) {
    selectAll();
    ui->leFio->setText(ui->twPatient->item(0,0)->text());
    ui->lePhoneNumber->setText(ui->twPatient->item(0,1)->text());
    ui->leEmail->setText(ui->twPatient->item(0,2)->text());
    QDate date = QDate::fromString(ui->twPatient->item(0, 3)->text(), "yyyy-MM-dd");
    ui->deDateOfBirth->setDate(date);
}

void patientList::selectAll()
{
    // Очистить содержимое компонента
    ui->twPatient->clearContents();
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from patient";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if( query.isActive())
        ui->twPatient->setRowCount( query.size());
    else
        ui->twPatient->setRowCount( 0);
    int i=0;
    while(query.next())
    {
        ui->twPatient->setItem(i,0,new
QTableWidgetItem(query.value("fio").toString()));
        ui->twPatient->setItem(i,1,new
QTableWidgetItem(query.value("phone_number").toString()));
        ui->twPatient->setItem(i,2,new
QTableWidgetItem(query.value("email").toString()));
        ui->twPatient->setItem(i,3,new
QTableWidgetItem(query.value("date_of_birth").toString()));
        i++;
    }
}

void patientList::add()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into patient(fio,phone_number,email,date_of_birth) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    query.bindValue(0,ui->leFio->text());
    query.bindValue(1,ui->lePhoneNumber->text());
    query.bindValue(2,ui->leEmail->text());
    query.bindValue(3,ui->deDateOfBirth->date());
    if( !query.exec() )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    // и обновить записи в компоненте таблицы
    selectAll();
}

void patientList::del()
{
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twPatient->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    if( QMessageBox::question(this,"Delete","Delete row?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    //!!! Здесь удаляем по ФИО но можно исправить !!!
    QString sqlstr = "delete from patient where fio = '"
            + ui->twPatient->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
   // Обновить содержимое компонента таблицы
    selectAll();
    ui->leFio->clear();
    ui->leEmail->clear();
    ui->lePhoneNumber->clear();
    ui->deDateOfBirth->clear();
}

void patientList::update() {
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twPatient->currentRow();
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
    QString fio_up = ui->leFio->text();
    QString phoneNumber_up = ui->lePhoneNumber->text();
    QString email_up = ui->leEmail->text();
    QString dateOfBirth_up = ui->deDateOfBirth->text();

    QSqlQuery query(dbconn);
    QString sqlstr = "update patient set fio='"+fio_up+"', phone_number='"+phoneNumber_up+"', email='"+email_up+"', date_of_birth='"+dateOfBirth_up+"'  where fio = '"
            + ui->twPatient->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    // Обновить содержимое компонента таблицы
    selectAll();
}


patientList::~patientList()
{
    if( dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void patientList::on_btnClear_clicked()
{
    ui->leFio->clear();
    ui->leEmail->clear();
    ui->lePhoneNumber->clear();
    ui->deDateOfBirth->clear();
}

void patientList::on_btnBack_clicked()
{
    this->close();
    adminmenu *amenu = new adminmenu(this);
    amenu->show();
}
