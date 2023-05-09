#include "chat.h"
#include "ui_chat.h"
#include<QListWidget>
#include"botgram.h"
#include<QMessageBox>
#include<QPushButton>


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
       ui->photo_button->hide();
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
   ui->photo_button->setStyleSheet("QPushButton {"
                                   "border: none;"
                                   "background-image: url(:/icons/pin.png);"
                                   "background-position: center;"
                                   "background-color: rgb(255, 255, 127);"
                                   "padding: 0px 10px 0px 10px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #E6E6E6;"
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
   ui->photo_button->show();

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
     /*  QTime time = QTime::currentTime();
       QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
       QString messageWithTime = QString("[%1] %2").arg(timeString).arg(message);
   ui->message_text->setStyleSheet("background-color: rgb(85, 255, 127);");
   ui->message_text->clear();

   QListWidgetItem* item = new QListWidgetItem(messageWithTime);
   item->setTextColor(Qt::black); // set text color to red
   item->setBackgroundColor(Qt::blue);
   if (message.length() < 10) {
       item->setBackgroundColor(Qt::blue);
   } else {
       item->setBackgroundColor(Qt::darkBlue);
   }
   ui->listWidget_2->addItem(item);
   ui->listWidget_2->scrollToBottom();*/
      /* QTime time = QTime::currentTime();
       QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
       QString messageWithTime = QString("[%1] %2").arg(timeString).arg(message);
       ui->message_text->setStyleSheet("background-color: rgb(85, 255, 127);");
       ui->message_text->clear();

       // Create a new QListWidgetItem with the message and time
       QListWidgetItem* itemWithTime = new QListWidgetItem(messageWithTime);
       itemWithTime->setTextColor(Qt::black); // set text color to black

       // Add the new item to listWidget1
       ui->listWidget->addItem(itemWithTime);

       // Add the item to listWidget2
       QListWidgetItem* item = new QListWidgetItem(messageWithTime);
       item->setTextColor(Qt::black); // set text color to black

       if (message.length() < 10) {
           item->setBackgroundColor(Qt::blue);
       } else {
           item->setBackgroundColor(Qt::darkBlue);
       }

       ui->listWidget_2->addItem(item);
       ui->listWidget_2->scrollToBottom();*/

       // Create a new QListWidgetItem with the message and time
       QTime time = QTime::currentTime();
       QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
       QString messageWithTime = QString("[%1] %2").arg(timeString).arg(message);
       QListWidgetItem* item = new QListWidgetItem(messageWithTime);
       ui->message_text->clear();
       item->setTextColor(Qt::black); // set text color to black

       if (message.length() < 10) {
           item->setBackgroundColor(Qt::blue);
       } else {
           item->setBackgroundColor(Qt::blue);
       }

       ui->listWidget_2->addItem(item);
       ui->listWidget_2->scrollToBottom();

       // Delete the second and subsequent items in listWidget1 and create a new item with the new time
       while (ui->listWidget->count() > 1) {
           QListWidgetItem* previousItem = ui->listWidget->takeItem(1);
           delete previousItem;
       }

       QListWidgetItem* newItem = new QListWidgetItem(messageWithTime);
       newItem->setTextColor(Qt::black); // set text color to black
       ui->listWidget->addItem(newItem);



    }

}

