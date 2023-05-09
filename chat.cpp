#include "chat.h"
#include "ui_chat.h"
#include<QListWidget>
#include"botgram.h"
#include<QMessageBox>


chat::chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat)
{

   // QString message;
    ui->setupUi(this);
    QDir cur = QDir::current();
    cur.cdUp();
    cur.cd("BotGram");
    cur.cd("DataBases");
    QFile savenamechat;
    QString temp = cur.path()+"/savenamechat.btg";
    savenamechat.setFileName(temp);
   if(!savenamechat.open(QIODevice::ReadOnly))
   {
       QMessageBox::information(this,"warning",savenamechat.errorString(),"ok");
   }
   else
   {
       QByteArray name;
       name=savenamechat.readAll();
       savenamechat.close();
       ui->message_text->hide();
       ui->pushButton_send_message->hide();
       ui->listWidget_2->hide();
       ui->label_selectchat->hide();
       //ui->listWidget_2->hide();
       QListWidget *listWidget = ui->listWidget;
       QListWidgetItem* item = new QListWidgetItem(name);
       QFont font = item->font();
       font.setBold(true); // set font to bold
       item->setFont(font);
       item->setTextColor(Qt::white);
       item->setBackgroundColor(Qt::red);
       ui->listWidget->addItem(item);
       //listWidget->addItem(name);
   }
   ui->pushButton_send_message->setStyleSheet("QPushButton {"
                                              "background-color: #54A8FF;"
                                              "border: none;"
                                              "border-radius: 4px;"
                                              "color: #FFFFFF;"
                                              "font-size: 14px;"
                                              "padding: 8px 12px;"
                                              "}"
                                              "QPushButton:hover {"
                                              "background-color: #4D98E8;"
                                              "}");
}

chat::~chat()
{
    delete ui;
}

void chat::on_listWidget_itemClicked(QListWidgetItem *item)
{
   ui->message_text->show();
   ui->pushButton_send_message->show();
   //ui->label_selectchat->hide();
   ui->label->hide();
   ui->listWidget_2->show();
   ui->label_selectchat->show();

}


void chat::on_pushButton_send_message_clicked()
{

   QString message = ui->message_text->toPlainText();
   if(message == "")
   {
       ui->message_text->setStyleSheet("background-color: rgb(255, 0, 0);");

   }
   else
   {
   //ui->listWidget_2->show();
   ui->message_text->clear();
   QListWidgetItem* item = new QListWidgetItem(message);
   item->setTextColor(Qt::black); // set text color to red
   item->setBackgroundColor(Qt::blue);
   ui->listWidget_2->addItem(item);
   }

}

