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
    ui->frame_alarm_3->setVisible(false);
    ui->txt_email->setVisible(false);

    ui->frame_Verify->setGeometry(ui->txt_email->geometry().x(),
                                  ui->txt_email->geometry().y(),
                                  ui->frame_Verify->geometry().width(),
                                  ui->frame_Verify->geometry().height());
    dataBase_complex db;
    xml_node<>* root=db.connectToXml("BotGramData.xml");
    xml_node<>* node=root->first_node("app_vars");
    if(strcmp(node->first_node("first_entering")->value(),"1")==0)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else ui->stackedWidget->setCurrentIndex(1);
}
bool botgram::get_LoginVar()
{
    return IsInLogin;
}

void botgram::set_LoginVar(bool v_Login)
{
    IsInLogin=v_Login;
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
        else if(Error==Account::USERNAME_IS_REPETITIVE ||
                Error==Account::USERNAME_IS_NOT_STANDARD ||
                Error==Account::USERNAME_NOT_FOUND)
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
    case Account::EMAIL:
    {
        ui->frame_alarm_3->setVisible(true);
        ui->lbl_alarm3->setVisible(true);
        ui->lbl_alarm3->setText(QString::fromStdString(TempStr));
        if(Error==Account::EMAIL_IS_NOT_EMAIL)
        {
            ui->lbl_pic_alarm3->setStyleSheet("background-color:red;"
                                              " border-radius:5px;");
        }
        else if(Error==Account::IS_CORRECT)
        {
            ui->lbl_pic_alarm3->setStyleSheet("background-color:green;"
                                              " border-radius:5px;");
        }
        break;
    }
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
    ui->txt_password->setStyleSheet("border-radius: 20px;"
                                    " background-color:#00000000 ;"
                                    " border: 1px solid;"
                                    "border-color: rgb(131, 155, 151);"
                                    "padding-left:10px;");
    Account myAccount;
    int Error=myAccount.checkCorrect_Text(ui->txt_username->text().toStdString(),Account::USERNAME);
    fixErrorinAlarmLabel(Error,Account::USERNAME);


}

void botgram::on_txt_password_textChanged(const QString &arg1)
{
    ui->txt_password->setStyleSheet("border-radius: 20px;"
                                    " background-color:#00000000 ;"
                                    " border: 1px solid;"
                                    "border-color: rgb(131, 155, 151);"
                                    "padding-left:10px;");
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
    Account mac;
    int ErrorUsername=mac.checkCorrect_Text(ui->txt_username->text().toStdString().c_str(),Account::USERNAME);
    int ErrorPassword=mac.checkCorrect_Text(ui->txt_password->text().toStdString().c_str(),Account::PASSWORD);

    if(!ErrorUsername && !ErrorPassword)
    {
        User_DataBase Udb;
        if(get_LoginVar()==true)
        {


            xml_node<>* root=Udb.connectToXml("sample.xml");
            if(root)
            {
                xml_node<>* MyUsernameNode=Udb.search(ui->txt_username->text().toStdString(),"username");
                if(!MyUsernameNode)
                {
                    fixErrorinAlarmLabel(Account::USERNAME_NOT_FOUND,Account::USERNAME);
                }
                else // username is correct
                {
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
                    else
                    {

                        ui->txt_password->setStyleSheet("border-radius: 20px;"
                                                        " background-color:#00000000 ;"
                                                        " border: 1px solid;"
                                                        "border-color: #dc0000;"
                                                        "padding-left:10px;");
                    }
                }
            }
            else
            {
                sendMessage("not found root\n or   added new file\n or is a Error ");
            }

        }
        else//Sign in
        {
            int ErrorEmail=mac.checkCorrect_Text(ui->txt_email->text().toStdString().c_str(),Account::EMAIL);
            if(!ErrorEmail)
            {
                dataBase_complex Perso;
                xml_node<>* rootPerso=Perso.connectToXml("BotGramData.xml");
                xml_node <>* newUser=Udb.addUser(ui->txt_username->text().toStdString(),
                                                 ui->txt_password->text().toStdString(),
                                                 ui->txt_email->text().toStdString());
                if(!newUser)
                {
                    sendMessage("Error in Add new User");
                    exit(1);
                }

                newUser->first_node("logined")->value("1");

                rootPerso->first_node("app_vars")->first_node("first_entering")->value("0");

                ui->stackedWidget->setCurrentIndex(2);

//                string fileMeStr;
//                print(back_inserter(fileMeStr),Udb.doc);
//                sendMessage(fileMeStr);

                Perso.save_modifies();
//                Udb.save_modifies();
            }
            else fixErrorinAlarmLabel(ErrorEmail,Account::EMAIL);

        }

    }
    else
    {
        ui->frame_alarm->setVisible(true);
        ui->frame_alarm_2->setVisible(true);
        if(ErrorUsername)
        {
            ui->lbl_Alarm->setText("fill username Correct!!!");
            ui->lbl_pic_alarm->setStyleSheet("background-color:red;"
                                             " border-radius:5px;");
        }
        else
        {
            fixErrorinAlarmLabel(ErrorUsername,Account::USERNAME);
        }
        if(ErrorPassword)
        {
            ui->lbl_alarm2->setText("fill password Correct!!!");
            ui->lbl_pic_alarm2->setStyleSheet("background-color:red;"
                                              " border-radius:5px;");
        }
        else
        {
            fixErrorinAlarmLabel(ErrorPassword,Account::PASSWORD);
        }
    }

}




void botgram::on_btn_SignIn_pressed()
{
    ui->btn_SignIn->setStyleSheet("text-decoration:underline ;"
                                  "border-radius:1px;");
}

void botgram::on_btn_SignIn_released()
{
    ui->btn_SignIn->setStyleSheet("border-radius:1px;");
}

void botgram::on_btn_SignIn_clicked()
{
    if(get_LoginVar())
    {
        set_LoginVar(false);
        ui->txt_email->setVisible(true);
        ui->frame_Verify->setGeometry(ui->frame_Verify->geometry().x(),
                                      ui->frame_Verify->geometry().y()+2*ui->txt_email->geometry().height(),
                                      ui->frame_Verify->geometry().width(),
                                      ui->frame_Verify->geometry().height());
        ui->btn_verify->setText("Sign In");
        ui->btn_SignIn->setText("Back");
    }
    else
    {
        set_LoginVar(true);
        ui->txt_email->setVisible(false);
        ui->frame_Verify->setGeometry(ui->frame_Verify->geometry().x(),
                                      ui->frame_Verify->geometry().y()-2*ui->txt_email->geometry().height(),
                                      ui->frame_Verify->geometry().width(),
                                      ui->frame_Verify->geometry().height());
        ui->btn_verify->setText("Login");
        ui->btn_SignIn->setText("Sign In");
    }
}
