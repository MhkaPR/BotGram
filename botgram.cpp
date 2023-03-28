#include "botgram.h"
#include "ui_botgram.h"
#include <QMessageBox>
#include <QWidget>
#include "libraries_BotGram/database/database_complex.h"
#include "libraries_BotGram/Accont/Account.h"



botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
{
    ui->setupUi(this);
    ui->frame_alarm->setVisible(false);
    ui->frame_alarm_2->setVisible(false);
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

void botgram::fixErrorinAlarmLabel(int Error, int fixFor)
{
    Account temp;
    string TempStr=temp.ErrorStr(Error);

    switch (fixFor) {
    case Account::USERNAME:
    {
        ui->frame_alarm->setVisible(true);
        ui->lbl_Alarm->setText(QString::fromStdString(TempStr));
        if(Error==Account::USERNAME_IS_SHORT)
        {
           ui->lbl_pic_alarm->setStyleSheet("background-color:yellow;"
                                           " border-radius:5px;");
        }
        else if(Error==Account::USERNAME_IS_REPETITIVE || Error==Account::USERNAME_IS_NOT_STANDARD)
        {
            ui->lbl_pic_alarm->setStyleSheet("background-color:red;"
                                            " border-radius:5px;");
        }
        else if(Error==Account::IS_CORRECT)
        {
            ui->lbl_pic_alarm->setStyleSheet("background-color:green;"
                                            " border-radius:5px;");
        }
        break;
    }
    case Account::PASSWORD:
    {
        ui->frame_alarm_2->setVisible(true);
        ui->lbl_alarm2->setVisible(true);
        ui->lbl_alarm2->setText(QString::fromStdString(TempStr));
        if(Error==Account::PASSWORD_IS_SHORT)
        {
           ui->lbl_pic_alarm2->setStyleSheet("background-color:yellow;"
                                           " border-radius:5px;");
        }
        else if(Error==Account::PASSWORD_IS_WEAK || Error==Account::PASSWORD_IS_NOT_STANDARD)
        {
            ui->lbl_pic_alarm2->setStyleSheet("background-color:red;"
                                            " border-radius:5px;");
        }
        else if(Error==Account::IS_CORRECT)
        {
            ui->lbl_pic_alarm2->setStyleSheet("background-color:green;"
                                            " border-radius:5px;");
        }

        break;
    }
    case Account::EMAIL:;
    }

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
    Account myAccount;
    int Error=myAccount.setUsername(ui->txt_username->text().toStdString());
    fixErrorinAlarmLabel(Error,Account::USERNAME);


}

void botgram::on_txt_password_textChanged(const QString &arg1)
{
    Account myAccount;
   int Error= myAccount.setPassword(ui->txt_password->text().toStdString());
    fixErrorinAlarmLabel(Error,Account::PASSWORD);

}

void botgram::on_txt_email_textChanged(const QString &arg1)
{
    Account myAccount;
   int Error= myAccount.setEmail(ui->txt_email->text().toStdString());
   fixErrorinAlarmLabel(Error,Account::EMAIL);

}

void botgram::on_btn_verify_clicked()
{
    //    string temp=myAccount.getUsername()+'\n';
    //    temp.append(myAccount.getPassword()+'\n');
    //    temp.append(myAccount.getEmail()+'\n');
    //    sendMessage(temp);
    QPushButton *usernameInfo=new QPushButton();
    usernameInfo->setGeometry(ui->txt_username->geometry().x(),
                              ui->txt_username->geometry().y()+ui->txt_username->geometry().height()+5,
                              ui->txt_username->geometry().width(),
                              ui->txt_username->geometry().height());
    usernameInfo->setText("Hello");
    //   usernameInfo->setStyleSheet("background-color:red;");




}

