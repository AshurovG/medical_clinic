#include "authorization.h"
#include "ui_authorization.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

authorization::authorization(QWidget *parent):
    QDialog(parent),
    ui(new Ui::authorization)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/61802.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    m_errorLabel = new QLabel();
    m_errorLabel->setStyleSheet("QLabel { color : red; }");
    ui->lePassword->setEchoMode(QLineEdit::Password);
    admin_window = new adminmenu();
    connect(admin_window,&adminmenu::firstWindow,this,&authorization::show);//сигнал на возврат на главное окно

    doctor_window = new doctormenu();
    connect(doctor_window,&doctormenu::firstWindow,this,&authorization::show);//сигнал на возврат на главное окно

    patient_window = new patientmenu();
    connect(patient_window,&patientmenu::firstWindow,this,&authorization::show);
}


authorization::~authorization()
{
    delete ui;
}

void authorization::on_btnEnter_clicked()
{
    QString login = ui->leLogin->text();
    QString password = ui->lePassword->text();
    if(login=="admin" && password=="111"){
        admin_window->show();
        this->close();
    } else if(login=="doctor" && password=="222"){
        doctor_window->show();
        this->close();
    } else if(login=="patient" && password=="333"){
        patient_window->show();
        this->close();
    } else if (login.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"Ошибка","Все поля должны быть заполнены!");
        return;
    } else {
        QMessageBox::warning(this,"Ошибка","Неверный логин или пароль!");
        return;
    }

    if(!dbconn.isOpen())
        {
            dbconn=QSqlDatabase::addDatabase("QPSQL");
            // Установить параметры соединения: имя БД, адрес хоста, логин и пароль
            dbconn.setDatabaseName("medical_clinic");
            dbconn.setHostName("localhost");
            dbconn.setUserName("student");
            dbconn.setPassword("1");
            // Открыть соединениe и результат вывести в окно вывода
            if( dbconn.open() )
                accept();
        }
        else
        // Если соединение уже открыто, то сообщить об этом
        m_errorLabel->setText("Connect is already open...");
}
