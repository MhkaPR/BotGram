#ifndef BOTGRAM_H
#define BOTGRAM_H

#include <QMainWindow>
#include <iostream>
#include <QMessageBox>
#include "libraries_BotGram/Accont/Account.h"
#include "Forms/buildercapcha.h"


using std::string;
QT_BEGIN_NAMESPACE
namespace Ui { class botgram; }
QT_END_NAMESPACE

class botgram : public QMainWindow
{
    Q_OBJECT
public slots:
    bool get_LoginVar();
    void set_LoginVar(bool);

    int get_CodeVar();
    void set_CodeVar(int);

    inline int BuildCodeVerify();
public:
     QString codeCaptcha;
    botgram(QWidget *parent = nullptr);
    void sendMessage(string);
    void fixErrorinAlarmLabel(int,/*ERROR Type*/int);
    ~botgram();

private slots:
    void on_StartApplication_btn_clicked();

    void on_btn_verify_pressed();

    void on_btn_verify_released();

    void on_txt_username_textChanged(const QString &arg1);

    void on_txt_password_textChanged(const QString &arg1);

    void on_txt_email_textChanged(const QString &arg1);

    void on_btn_verify_clicked();




    void on_btn_SignIn_pressed();

    void on_btn_SignIn_released();

    void on_btn_SignIn_clicked();



    void on_btn_checkVerifyCode_clicked();

    void on_pushButton_clicked();


    void on_txt_password_textEdited(const QString &arg1);

    void on_eye_btn_pressed();

    void on_eye_btn_released();

    void txt_capcha_clean();

private:
    Ui::botgram *ui;
    bool IsInLogin=true;
    int CodeVerify=0;
    Account account;
    bool switch_eye_btn_in_loginForm=false;
    BuilderCapcha *bCaptcha=nullptr;


};
#endif // BOTGRAM_H
