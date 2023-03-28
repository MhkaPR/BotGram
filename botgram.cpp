#include "botgram.h"
#include "ui_botgram.h"
#include <QMessageBox>
#include <QWidget>
#include "libraries_BotGram/database/database_complex.h"
#include "libraries_BotGram/Accont/Account.h"
#include "libraries_BotGram/database/user_database.h"



botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
{
    ui->setupUi(this);
    ui->frame_alarm->setVisible(false);
    ui->frame_alarm_2->setVisible(false);
    ui->txt_email->setVisible(false);

    ui->btn_verify->setGeometry(ui->txt_email->geometry().x(),
                                ui->txt_email->geometry().y(),
                                ui->btn_verify->geometry().width(),
                                ui->btn_verify->geometry().height());
    dataBase_complex db;
    xml_node<>* root=db.connectToXml("BotGramData.xml");
    xml_node<>* node=root->first_node("app_vars");
    if(strcmp(node->first_node("first_entering")->value(),"1")==0)
    {
        ui->stackedWidget->setCurrentIndex(2);
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
    int Error=myAccount.checkCorrect_Text(ui->txt_username->text().toStdString(),Account::USERNAME);
    fixErrorinAlarmLabel(Error,Account::USERNAME);


}

void botgram::on_txt_password_textChanged(const QString &arg1)
{
    Account myAccount;
    int Error= myAccount.checkCorrect_Text(ui->txt_password->text().toStdString(),Account::PASSWORD);
    fixErrorinAlarmLabel(Error,Account::PASSWORD);

}

void botgram::on_txt_email_textChanged(const QString &arg1)
{
    Account myAccount;
    int Error= myAccount.checkCorrect_Text(ui->txt_email->text().toStdString(),Account::EMAIL);
    fixErrorinAlarmLabel(Error,Account::EMAIL);

}

void botgram::on_btn_verify_clicked()
{
    if(!ui->txt_email->isVisible())
    {
        if(ui->lbl_Alarm->text()=="it's correct :)" && ui->lbl_alarm2->text()=="it's correct :)")
        {

            User_DataBase Udb;
            xml_node<>* root=Udb.connectToXml("sample.xml");
            if(root)
            {
                xml_node<>* MyUsernameNode=Udb.search(ui->txt_username->text().toStdString());
                if(!MyUsernameNode)
                {
                    sendMessage("Not found such user!");
                    ui->txt_email->setVisible(true);
                    ui->btn_verify->setGeometry(ui->btn_verify->geometry().x(),
                                                ui->btn_verify->geometry().y()+2*ui->txt_email->geometry().y(),
                                                ui->btn_verify->geometry().width(),
                                                ui->btn_verify->geometry().height());


                }
                else {
                    if(strcmp(MyUsernameNode->first_node("password")->value(),ui->txt_password->text().toStdString().c_str())==0)
                    {
                        MyUsernameNode->first_node("logined")->value("1");
                        //codes must be chenged
                        dataBase_complex Perso;
                        xml_node<>* rootPerso=Perso.connectToXml("BotGramData.xml");
                        rootPerso->first_node("app_vars")->first_node("first_entering")->value("1");
                        ui->stackedWidget->setCurrentIndex(2);

                        Perso.save_modifies();
                        Udb.save_modifies();

                    }
                }
            }
            else
            {
                sendMessage("not found root\n or   added new file\n or is a Error ");
            }
        }
        else {
            ui->lbl_Alarm->setText("fill username Correct!!!");
            ui->lbl_pic_alarm->setStyleSheet("background-color:red;"
                                             " border-radius:5px;");
            ui->lbl_alarm2->setText("fill password Correct!!!");
            ui->lbl_pic_alarm2->setStyleSheet("background-color:red;"
                                              " border-radius:5px;");
        }
    }
    else
    {

    }

}

