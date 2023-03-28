#include "botgram.h"
#include "ui_botgram.h"
#include <QMessageBox>
#include <QWidget>
#include "libraries_BotGram/database/database_complex.h"
#include "libraries_BotGram/Accont/Account.h"

Account myAccount;

botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
{
    ui->setupUi(this);
    ui->txt_email->setVisible(false);
    ui->lbl_capcha->setVisible(false);
    ui->btn_verify->setGeometry(ui->txt_email->geometry().x(),
                                ui->txt_email->geometry().y(),
                                ui->btn_verify->geometry().width(),
                                ui->btn_verify->geometry().height());
    dataBase_complex db;
    xml_node<>* root=db.connectToXml("sd");
    xml_node<>* node=root->first_node("app_vars");
    if(strcmp(node->first_node("first_entering")->value(),"1")==0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void botgram::sendMessage(string str)
{
    QMessageBox msg;
    msg.setText(QString::fromStdString(str));
    msg.show();
    msg.exec();
}

botgram::~botgram()
{
    delete ui;
}


void botgram::on_StartApplication_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void botgram::on_btn_verify_pressed()
{
    ui->btn_verify->setStyleSheet("color: white;"
                                  "border-radius:25px;"
                                  "background-color:#2097b5");

}

void botgram::on_btn_verify_released()
{
    ui->btn_verify->setStyleSheet(
                " color: rgb(131, 155, 166);"
                "  border-radius:25px;"
                "  border:1px solid;"
                " border-color: rgb(131, 155, 151);");
}

void botgram::on_txt_username_textChanged(const QString &arg1)
{

    myAccount.setUsername(ui->txt_username->text().toStdString());


}

void botgram::on_txt_password_textChanged(const QString &arg1)
{

    myAccount.setPassword(ui->txt_password->text().toStdString());

}

void botgram::on_txt_email_textChanged(const QString &arg1)
{
     myAccount.setEmail(ui->txt_email->text().toStdString());
}

void botgram::on_btn_verify_clicked()
{
//    string temp=myAccount.getUsername()+'\n';
//    temp.append(myAccount.getPassword()+'\n');
//    temp.append(myAccount.getEmail()+'\n');
//    sendMessage(temp);

}
