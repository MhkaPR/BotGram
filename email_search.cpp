#include "email_search.h"
#include "ui_email_search.h"
#include"libraries_BotGram/searchuser.h"

email_search::email_search(QTcpSocket *so,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::email_search)
{
    ui->setupUi(this);
    socket=so;
}

email_search::~email_search()
{
    delete ui;
}

void email_search::on_pushButton_clicked()
{
    searchUserPackat search_user;
    search_user.setemail(ui->lineEdit->text());
    QByteArray buff2;
    QDataStream out2(&buff2,QIODevice::WriteOnly);
    out2.setVersion(QDataStream::Qt_4_0);
    out2<<static_cast<short>(search_user.getheader())<<search_user.serialize();
    socket->write(buff2);
    socket->waitForBytesWritten();
    this->hide();

}

