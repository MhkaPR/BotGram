#ifndef BOTGRAM_H
#define BOTGRAM_H

#include <QMainWindow>
#include <iostream>
#include <QMessageBox>
using std::string;
QT_BEGIN_NAMESPACE
namespace Ui { class botgram; }
QT_END_NAMESPACE

class botgram : public QMainWindow
{
    Q_OBJECT

public:
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



private:
    Ui::botgram *ui;
};
#endif // BOTGRAM_H
