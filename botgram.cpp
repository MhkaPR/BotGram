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
#include "chat.h"
#include<QApplication>
#include"libraries_BotGram/systemmessagepacket.h"
#include"libraries_BotGram/checkverifysafepacket.h"
#include"libraries_BotGram/adduser_spacket.h"
#include"libraries_BotGram/tokenpacket.h"

#define SERVER_PORT 9999

botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
    ,bCaptcha(new BuilderCapcha)

{

    ui->setupUi(this);

    me.connectToHost(QHostAddress::LocalHost,SERVER_PORT);
    ui->frame_alarm->setVisible(false);
    ui->frame_alarm_2->setVisible(false);
    ui->frame_alarm_3->setVisible(false);
    ui->txt_email->setVisible(false);


    ui->frame_Verify->setGeometry(ui->txt_email->geometry().x(),
                                  ui->txt_email->geometry().y(),
                                  ui->frame_Verify->geometry().width(),
                                  ui->frame_Verify->geometry().height());

    QDir cur=QDir::current();
    //    cur.cdUp();
    //    cur.cd("botGram");
    string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
    ifstream Doc(tempAdd.c_str());
    string check_firstEntering;
    Doc >> check_firstEntering;
    if(check_firstEntering == "1")
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
    else ui->stackedWidget->setCurrentIndex(0);
    Doc.close();

    ui->wi_capcha->layout()->addWidget(bCaptcha);
    ui->wi_capcha->setCurrentWidget(bCaptcha);

    codeCaptcha = bCaptcha->strCaptcha;
    connect(bCaptcha,&BuilderCapcha::refreshedCode,[=](){
         codeCaptcha = bCaptcha->strCaptcha;
    } );   //connect(bCaptcha,&BuilderCapcha::on_btn_again_clicked,this,&botgram::txt_capcha_clean);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("botgramdatabase.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Failed to open database!";
        return;
    }



    //3 - list mokatabin

}
bool botgram::get_LoginVar()
{
    return IsInLogin;
}

void botgram::set_LoginVar(bool v_Login)
{
    IsInLogin=v_Login;
}

QString botgram::get_CodeVar()
{
    return CodeVerify;
}

void botgram::set_CodeVar(QString your_Code)
{
    CodeVerify=your_Code;
}

int botgram::BuildCodeVerify()
{
    /* srand(time(0));
    return  (rand()%90000+9999);*/

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
        else if (Error==Account::EMAIL_HAVE_BAD_CHARS) {

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
    delete this;
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
    int Errorcaptcha = (ui->txt_captcha->text() == codeCaptcha) ? 0 : 1 ;
   //sendMessage(codeCaptcha.toStdString());
    if(!ErrorUsername && !ErrorPassword && !Errorcaptcha )
    {
        // User_DataBase Udb;
        // xml_document<>* Doc=Udb.connectToXml("sample.xml");
        if(get_LoginVar()==true)
        {
            loginPacket p;

            QJsonDocument doc;
            QJsonObject obj;
            obj.insert("header",static_cast<short>(p.getheader()));
            obj.insert("username",ui->txt_username->text());
            obj.insert("password",ui->txt_password->text());
            obj.insert("islogin",true);
            doc.setObject(obj);
            //  p.JsonInformation = doc.toJson();
            p.setJsonLoginData(doc.toJson());

            QByteArray buf;
            QDataStream out(&buf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);

            out << static_cast<short>(p.getheader())<<p.serialize();

            me.write(buf);
            me.waitForReadyRead();


            QByteArray answerBuf;
            QDataStream in(&answerBuf,QIODevice::ReadOnly);
            in.setVersion(QDataStream::Qt_4_0);

            answerBuf = me.readAll();
            systemMessagePacket sys;
            sys.deserialize(answerBuf);

            //in >>sys;
            //QMessageBox::information(this,"wd",QString::number(static_cast<short>(sys.getSysmsg())));
            if(sys.getSysmsg()  == package::login_confrimed)
            {
                QDir cur=QDir::current();
                //                        cur.cdUp();
                //                        cur.cd("botGram");
                string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
                fstream DocPerso(tempAdd.c_str());
                DocPerso << "1";
                DocPerso.close();



                QJsonDocument jsondoc = QJsonDocument::fromJson(sys.getinformation());
                QJsonObject jsonobj;
                jsonobj =jsondoc.object();
                db.close();
                db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName("botgramdatabase.db");

                // Open the database
                if (!db.open()) {
                    qDebug() << "Failed to open database!";
                    return;
                }



                QSqlQuery query(db);
                 query.prepare("UPDATE myinformation SET username=:u , password=:p , email=:e , name=:n , token=:t");

                query.bindValue(":u",ui->txt_username->text() );
                query.bindValue(":p",ui->txt_password->text() );
                query.bindValue(":e",jsonobj["email"].toString());
                query.bindValue(":n",jsonobj["name"].toString());
                query.bindValue(":t",jsonobj["token"].toString());

                // Execute the query
                if (!query.exec()) {
                    QMessageBox::information(this,"warning",query.lastError().text(),"ok");

                    return;
                }
                query.finish();
                db.commit();
                chat *w3 = new chat;
                w3->setWindowTitle("chat page");
                w3->resize(1310,810);
                this->hide();
                w3->show();

            }
            //sendMessage(QString::number(sys.msg).toStdString());
            // me.connectToHost(QHostAddress::LocalHost,SERVER_PORT);
            /* if(Doc)
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

                        if(bCaptcha->strCaptcha == ui->txt_captcha->text())
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
                        }
                        else
                        {
                            ui->txt_captcha->setStyleSheet("border-radius: 20px;"
                                                           " background-color:#00000000 ;"
                                                           " border: 1px solid;"
                                                           "border-color: #dc0000;"
                                                           "padding-left:10px;");
                            ui->txt_captcha->setText("");
                            ui->txt_captcha->setPlaceholderText("Enter correct captcha");
                            bCaptcha->buildAgain();
                        }


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
            }*/

        }
        else//Sign in
        {

            int ErrorEmail =mac.checkCorrect_Text(ui->txt_email->text().toStdString().c_str(),Account::EMAIL);

            if(!ErrorEmail)
            {
                loginPacket SignInPacket;
                //send data to sign in
                QJsonDocument SignInDoc;
                QJsonObject ObjSign;
                ObjSign.insert("username",ui->txt_username->text());
                ObjSign.insert("password","");
                ObjSign.insert("email",ui->txt_email->text());
                ObjSign.insert("islogin",false);
                SignInDoc.setObject(ObjSign);
                QByteArray bufJ;
                QDataStream out(&bufJ,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_0);

                SignInPacket.setJsonLoginData(SignInDoc.toJson());

                // out << package::Packeting(SignInPacket.getheader(),SignInPacket.serialize());
                out << static_cast<short>(SignInPacket.getheader())<<SignInPacket.serialize();

                me.write(bufJ);
                me.waitForReadyRead();


                QByteArray ansBuf;
                QDataStream in(&ansBuf,QIODevice::ReadOnly);
                in.setVersion(QDataStream::Qt_4_0);


                ansBuf = me.readAll();

                systemMessagePacket sysAns;
                sysAns.deserialize(ansBuf);

                qDebug() << sysAns.getSysmsg();
                if(sysAns.getSysmsg() == package::s_email_is_repititive)
                {
                    ui->lbl_alarm3->setText("your email is repeatitive");
                   ui->lbl_pic_alarm3->setStyleSheet("background-color:red;"
                                                     " border-radius:5px;");
                }

                if(sysAns.getSysmsg() == package::s_send_apply_For_Link)
                {
                    sysAns.setSysmsg(package::Send_VerifyCode);
                    QByteArray CodeBuf;
                    QDataStream out2(&CodeBuf,QIODevice::WriteOnly);
                    out2.setVersion(QDataStream::Qt_4_0);

                    out2 <<  static_cast<short>(sysAns.getheader())<<sysAns.serialize();

                    me.write(CodeBuf);
                    //    sendmessage("wait for link...");
                    me.waitForReadyRead();

                    QByteArray ansBuf2;
                    QDataStream in2(&ansBuf2,QIODevice::ReadOnly);
                    in2.setVersion(QDataStream::Qt_4_0);

                    ansBuf2 = me.readAll();

                    CheckVerifySafePacket VerifyAns;
                    VerifyAns.deserialize(ansBuf2);
                    //in2 >> VerifyAns;
                    set_CodeVar(VerifyAns.getLink());


                    ui->stackedWidget->setCurrentIndex(2);
                    //  sendMessage(VerifyAns.Link.toStdString());
                    //   sendMessage((get_CodeVar()).toStdString());
                    QMessageBox::information(this,"DW",get_CodeVar(),"sf");


                    //-----------------------------------------------
                    /*int ErrorEmail=mac.checkCorrect_Text(ui->txt_email->text().toStdString().c_str(),Account::EMAIL);
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
                        if(bCaptcha->strCaptcha == ui->txt_captcha->text())
                        {

                            account.setUsername(ui->txt_username->text().toStdString());
                            account.setPassword(ui->txt_password->text().toStdString());
                            account.setEmail(ui->txt_email->text().toStdString());

                            ui->stackedWidget->setCurrentIndex(2);

                            int MyCode=BuildCodeVerify();
                            set_CodeVar(MyCode);

                            sendMessage("this is a verify code\n"
                                        "to your Sign in BotGram:\n"+to_string(MyCode));
                        }
                        else
                        {
                            ui->txt_captcha->setStyleSheet("border-radius: 20px;"
                                                           " background-color:#00000000 ;"
                                                           " border: 1px solid;"
                                                           "border-color: #dc0000;"
                                                           "padding-left:10px;");
                            ui->txt_captcha->setText("");
                            ui->txt_captcha->setPlaceholderText("Enter correct captcha");
                            bCaptcha->buildAgain();
                        }
                        Udb.save_modifies();
                    }
                }
                else
                {
                    fixErrorinAlarmLabel(Account::USERNAME_IS_REPETITIVE,Account::USERNAME);
                }

            } else fixErrorinAlarmLabel(ErrorEmail,Account::EMAIL);*/
                }
            }
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

    bCaptcha->buildAgain();
    ui->txt_captcha->setText("");
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
    QByteArray sendAnsAutho;
    QDataStream out3(&sendAnsAutho,QIODevice::WriteOnly);
    out3.setVersion(QDataStream::Qt_4_0);

    if(ui->txt_CodeText->text() == (get_CodeVar()))
    {
        QJsonDocument SignInDoc;
        QJsonObject ObjSign;
        ObjSign.insert("username",ui->txt_username->text());
        ObjSign.insert("password",ui->txt_password->text());
        ObjSign.insert("email",ui->txt_email->text());
        ObjSign.insert("islogin",0);
        SignInDoc.setObject(ObjSign);

        AddUser_SPacket user;
        user.setdata(SignInDoc.toJson());

        out3 <<static_cast<short>( user.getheader())<<user.serialize();


        me.write(sendAnsAutho);
        me.waitForReadyRead();

        QByteArray TokenBuf;
        QDataStream in(&TokenBuf,QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_0);

        TokenBuf = me. readAll();

        TokenPacket Tpac;
        Tpac.deserialize(TokenBuf);

        qDebug()<< Tpac.getToken();
        //in >> Tpac
        db.close();
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("botgramdatabase.db");

        // Open the database
        if (!db.open()) {
            qDebug() << "Failed to open database!";
            return;
        }


        QDir cur=QDir::current();
        QSqlQuery query(db);
        query.prepare("INSERT INTO myinformation (username, password,email,name,token) VALUES(:u,:p,:e,'',:t)");
        query.bindValue(":u",ui->txt_username->text() );
        query.bindValue(":p",ui->txt_password->text() );
        query.bindValue(":e",ui->txt_email->text());
        query.bindValue(":t",Tpac.getToken());
        // Execute the query
        if (!query.exec()) {
            QMessageBox::information(this,"warning",query.lastError().text(),"ok");

            return;
        }
        query.finish();
        db.commit();
        //        cur.cdUp();
        //        cur.cd("botGram");
        /*string tokenname = cur.path().toStdString() + "/DataBases/Token.btg";
        fstream Doctoken(tokenname);
        Doctoken << Tpac.getToken().toStdString().c_str();
        Doctoken.close();*/

        /* QFile TokenFile;
        TokenFile.setFileName("Token.btg");  //format is according to the name of our project
      if(!TokenFile.open(QIODevice::WriteOnly))
      {
          sendMessage("ERROR IN OPENING FILE");
          exit(1);

      }*/
        /*  QDir cur=QDir::current();
      cur.cdUp();
      cur.cd("botGram");*/
        string tempAdd = cur.path().toStdString() + "/DataBases/data.btg";
        fstream DocPerso(tempAdd.c_str());
        DocPerso << "1";
        DocPerso.close();
        ui->stackedWidget->setCurrentIndex(3);
        /*TokenFile.write((get_CodeVar()).toStdString().c_str());
      TokenFile.close();*/


        // sendMessage(Tpac.Token.toStdString());

    }

    else

    {
        sendMessage("your code is not correct");
    }



    /* int CodeMe=get_CodeVar();
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
    }*/
}
void botgram::on_pushButton_clicked()
{
    /*int Code=BuildCodeVerify();
    sendMessage(to_string(Code));
    set_CodeVar(Code);*/

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

void botgram::txt_capcha_clean()
{
    ui->txt_captcha->setText("");
}

void botgram::on_checkname_clicked()
{
    QString namechat;
    if(ui->name->text()=="")
    {
        sendMessage("Please enter a name for yourself");
    }
    else
    {
//        db.close();
//        db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("botgramdatabase.db");


        QSqlQuery query1(db);
        query1.prepare("SELECT username FROM myinformation");

        if (!query1.exec()) {
            QMessageBox::information(this,"warning",query1.lastError().text(),"ok");

            return;
        }
        if(query1.next())
        {
            qDebug() << query1.value("username").toString();
            QJsonDocument jsondoc;
            QJsonObject jsonobj;
            jsonobj.insert("name",ui->name->text());
            jsonobj.insert("username",query1.value("username").toString());
            jsondoc.setObject(jsonobj);

            systemMessagePacket syspacket;
            syspacket.setSysmsg(package::get_name);
            syspacket.setinformation(jsondoc.toJson());

            QByteArray changenamebuf;
            QDataStream out(&changenamebuf,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_0);
            out<<static_cast<short>(syspacket.getheader())<<syspacket.serialize();

            me.write(changenamebuf);
            me.waitForBytesWritten();





        }
        query1.finish();
        db.commit();

        db.close();
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("botgramdatabase.db");

        // Open the database
        if (!db.open()) {
            qDebug() << "Failed to open database!";
            return;
        }
        namechat = ui->name->text();
        QDir cur = QDir::current();
        //        cur.cdUp();
        //        cur.cd("BotGram");
        cur.cd("DataBases");
        QSqlQuery query(db);
        query.prepare("UPDATE myinformation SET name=:n");
        query.bindValue(":n",ui->name->text() );
        if (!query.exec()) {
            QMessageBox::information(this,"warning","query update"+query.lastError().text(),"ok");

            return;
        }
        query.finish();
        db.commit();
        db.close();


        me.disconnectFromHost();
        chat *w3 = new chat;
        w3->setWindowTitle("chat page");
        w3->resize(1310,810);
        this->hide();
        w3->show();


    }
}

