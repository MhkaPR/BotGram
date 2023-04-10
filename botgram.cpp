#include "botgram.h"
#include "ui_botgram.h"
#include <QMessageBox>
#include <QWidget>
#include "libraries_BotGram/database/database_complex.h"
#include "libraries_BotGram/Accont/Account.h"
#include "libraries_BotGram/database/user_database.h"
#include "botgrameenv.h"
#include "libraries_BotGram/Connection/clientHost.h"
#include "libraries_BotGram/Connection/serialize.h"
#include"libraries_BotGram/Handlers/RecvANDconnectionHandler.h"
#include "libraries_BotGram/capcha/capchacreator.h"
#include <QThread>
#include <QDir>
#include "capchabuilder.h"
#define SERVER_PORT 6969
clientHost c1;

botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
{

    // c1=clientHost("127.0.0.1",SERVER_PORT,Connection,RecvHandler);





    ui->setupUi(this);
    ui->frame_alarm->setVisible(false);
    ui->frame_alarm_2->setVisible(false);
    ui->frame_alarm_3->setVisible(false);
    ui->txt_email->setVisible(false);


    ui->frame_Verify->setGeometry(ui->txt_email->geometry().x(),
                                  ui->txt_email->geometry().y(),
                                  ui->frame_Verify->geometry().width(),
                                  ui->frame_Verify->geometry().height());

    QDir cur=QDir::current();
    cur.cdUp();
    cur.cd("botGram");
    string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
   ifstream Doc(tempAdd.c_str());
   string check_firstEntering;
   Doc >> check_firstEntering;
    if(check_firstEntering == "1")
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
    else ui->stackedWidget->setCurrentIndex(1);
    Doc.close();

}
bool botgram::get_LoginVar()
{


    return IsInLogin;

}

void botgram::set_LoginVar(bool v_Login)
{
    IsInLogin=v_Login;
}

int botgram::get_CodeVar()
{
    return CodeVerify;
}

void botgram::set_CodeVar(int your_Code)
{
    CodeVerify=your_Code;
}

inline int botgram::BuildCodeVerify()
{
    srand(time(0));
    return  (rand()%90000+9999);

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
        else if (Error==Account::EMAIL_IS_REPETITIVE) {

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
        xml_document<>* Doc=Udb.connectToXml("sample.xml");
        if(get_LoginVar()==true)
        {
            if(Doc)
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
                        QDir cur=QDir::current();
                        cur.cdUp();
                        cur.cd("botGram");
                        string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
                        fstream DocPerso(tempAdd.c_str());
                        DocPerso << "1";
                        DocPerso.close();

                        ui->stackedWidget->setCurrentIndex(3);


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
                sendMessage("not found Doc\n or   added new file\n or is a Error ");
            }

        }
        else//Sign in
        {
            int ErrorEmail=mac.checkCorrect_Text(ui->txt_email->text().toStdString().c_str(),Account::EMAIL);
            if(!ErrorEmail)
            {
                  //exit(0);
                xml_node<>* MyUsernameNode=Udb.search(ui->txt_username->text().toStdString(),"username");

                if(!MyUsernameNode)
                {

                    User_DataBase udb;
                    udb.connectToXml("sample.xml");

                    if(udb.search(ui->txt_email->text().toStdString(),"email"))
                    {
                        fixErrorinAlarmLabel(Account::EMAIL_IS_REPETITIVE,Account::EMAIL);
                    }
                    else
                    {

                        account.setUsername(ui->txt_username->text().toStdString());
                        account.setPassword(ui->txt_password->text().toStdString());
                        account.setEmail(ui->txt_email->text().toStdString());

                        ui->stackedWidget->setCurrentIndex(2);

                        int MyCode=BuildCodeVerify();
                        set_CodeVar(MyCode);

                        sendMessage("this is a verify code\n"
                                    "to your Sign in BotGram:\n"+to_string(MyCode));

                        //                string fileMeStr;
                        //                print(back_inserter(fileMeStr),Udb.doc);
                        //                sendMessage(fileMeStr);

                        //Perso.save_modifies();
                        Udb.save_modifies();
                    }
                }
                else
                {
                    fixErrorinAlarmLabel(Account::USERNAME_IS_REPETITIVE,Account::USERNAME);
                }

            } else fixErrorinAlarmLabel(ErrorEmail,Account::EMAIL);
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
        ui->frame_alarm_3->setVisible(true);
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
        ui->frame_alarm_3->setVisible(false);
        ui->frame_Verify->setGeometry(ui->frame_Verify->geometry().x(),
                                      ui->frame_Verify->geometry().y()-2*ui->txt_email->geometry().height(),
                                      ui->frame_Verify->geometry().width(),
                                      ui->frame_Verify->geometry().height());
        ui->btn_verify->setText("Login");
        ui->btn_SignIn->setText("Sign In");
    }
}
void botgram::on_btn_checkVerifyCode_clicked()
{
    int CodeMe=get_CodeVar();
    if(strcmp(ui->txt_CodeText->text().toStdString().c_str(),to_string(CodeMe).c_str())==0)
    {
        User_DataBase Udb;
        Udb.connectToXml("sample.xml");

        xml_node<>* newUser=Udb.addUser(account.getUsername(),account.getPassword(), account.getEmail());
        if(!newUser)
        {
            sendMessage("Error in Add new User");
            exit(1);
        }



        newUser->first_node("logined")->value("1");
        Udb.save_modifies();


        QDir cur=QDir::current();
        cur.cdUp();
        cur.cd("botGram");
        string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
        fstream DocPerso(tempAdd.c_str());
        if(!DocPerso.is_open())
        {
            sendMessage("Error in Add rootPerso");
            exit(1);
        }
        DocPerso  << "1";
        DocPerso.close();

        // sendMessage(rootPerso->first_node("app_vars")->first_node("first_entering")->value());

        ui->stackedWidget->setCurrentIndex(3);
    }
    else {
        sendMessage("Your Code is not Correct!");
    }
}
void botgram::on_pushButton_clicked()
{
    int Code=BuildCodeVerify();
    sendMessage(to_string(Code));
    set_CodeVar(Code);

}

void botgram::on_txt_password_textEdited(const QString &arg1)
{
    if(!switch_eye_btn_in_loginForm)
        ui->txt_password->setEchoMode(QLineEdit::Password);
    else
    {
        ui->txt_password->setEchoMode(QLineEdit::Normal);
    }
}

void botgram::on_eye_btn_pressed()
{
    QString temp = "   background-color:#00000000 ;"
                   " border: 0px solid;"
                   " color:rgb(255, 255, 255);"
                   " image: url(:/Eye/openEye.png);";
    ui->eye_btn->setStyleSheet(temp);
    ui->txt_password->setEchoMode(QLineEdit::Normal);
}

void botgram::on_eye_btn_released()
{
    QString temp = "   background-color:#00000000 ;"
                   " border: 0px solid;"
                   " color:rgb(255, 255, 255);"
                   " image: url(:/Eye/closedEye.png);";
    ui->eye_btn->setStyleSheet(temp);
    ui->txt_password->setEchoMode(QLineEdit::Password);

}
