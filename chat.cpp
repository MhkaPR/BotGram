#include "chat.h"
#include "ui_chat.h"
#include<QListWidget>
#include"botgram.h"
#include<QMessageBox>
#include<QPushButton>
#include<QFileDialog>
#include<QDesktopServices>
#include<QHBoxLayout>


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


       name=savenamechat.readAll();
       savenamechat.close();
       ui->message_text->hide();
       ui->label_onoroff->hide();
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
       item->setBackgroundColor(Qt::blue);
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

   ui->label_onoroff->show();

   //ui->label_selectchat->setText(name);
   //ui->label_selectchat->setAlignment(Qt::AlignHCenter);
   ui->label_selectchat->setText(name);
   ui->label_selectchat->setAlignment(Qt::AlignHCenter);



}


void chat::on_pushButton_send_message_clicked()
{
   QString message = ui->message_text->toPlainText();
   if(message.isEmpty())
   {
       ui->message_text->setStyleSheet("background-color: rgb(255, 0, 0);");
   }
   else
   {
       // Create a new QListWidgetItem with the message and time
       QTime time = QTime::currentTime();
       QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
       QString messageWithTime = QString("[%1] ").arg(timeString);

       // Split the message into chunks of 50 characters
       QStringList messageChunks;
       int messageLength = message.length();
       for (int i = 0; i < messageLength; i += 50) {
           QString chunk = message.mid(i, 50);
           messageChunks.append(chunk);
           messageWithTime += chunk;
           if (i + 50 < messageLength) {
               messageWithTime += "\n       ";
           }
       }

       QListWidgetItem* item = new QListWidgetItem(messageWithTime);
       ui->message_text->setStyleSheet("background-color: rgb(85, 255, 127);");
       ui->message_text->clear();

       // Set the background color based on the length of the message
       if (messageLength < 10) {
           item->setBackgroundColor(Qt::green);
       } else {
           item->setBackgroundColor(Qt::green);
       }

       // Add the item to listWidget2
       ui->listWidget_2->addItem(item);
       ui->listWidget_2->scrollToBottom();

       // Delete the second and subsequent items in listWidget1 and create a new item with the new time
       while (ui->listWidget->count() > 1) {
           QListWidgetItem* previousItem = ui->listWidget->takeItem(1);
           delete previousItem;
       }

       // Create a new item with the first two lines of the message and add it to listWidget1
       QStringList firstTwoLines = messageChunks.mid(0, 2);
       QString firstTwoLinesString = firstTwoLines.join("\n");
        messageWithTime = QString("[%1] ").arg(timeString);
       QListWidgetItem* newItem = new QListWidgetItem(QString("[%1] %2").arg(timeString,firstTwoLinesString));
       newItem->setTextColor(Qt::black); // set text color to black
       ui->listWidget->addItem(newItem);

       // Scroll to the bottom of the list widgets
       ui->listWidget->scrollToBottom();
       ui->listWidget_2->scrollToBottom();
   }
}



void chat::on_photo_button_clicked()
{
    // Open a file dialog to let the user choose an image file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose an image file"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));
    if (filePath.isEmpty()) {
        // If the user cancels the file dialog, return without doing anything
        return;
    }

    // Load the image from the file
    QImage image(filePath);
    if (image.isNull()) {
        // If the image cannot be loaded, show an error message and return
        QMessageBox::critical(this, tr("Error"), tr("Cannot load image file: %1").arg(filePath));
        return;
    }

    // Create a new QListWidgetItem with the image and time, and add it to listWidget_2
    QTime time = QTime::currentTime();
    QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
    QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)), QString("[%1] ").arg(timeString), ui->listWidget_2);
    item->setBackgroundColor(Qt::white);

    ui->listWidget_2->scrollToBottom();

    // Clear all items in listWidget except the first item
    while (ui->listWidget->count() > 1) {
        delete ui->listWidget->takeItem(1);
    }

    // Create a new QListWidgetItem with the text "photo" and the time, and add it to listWidget
    QListWidgetItem* newItem = new QListWidgetItem(QString("[%1] photo").arg(timeString), ui->listWidget);
    newItem->setBackgroundColor(Qt::white);

    ui->listWidget->scrollToBottom();
}












/*void chat::on_photo_button_clicked()
{
    // Open a file dialog to let the user choose an image or video file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"), QDir::homePath(), tr("Images and Videos (*.png *.xpm *.jpg *.mp4)"));
    if (filePath.isEmpty()) {
        // If the user cancels the file dialog, return without doing anything
        return;
    }

    // Check if the file is an image or a video
    QString extension = QFileInfo(filePath).suffix();
    if (extension == "png" || extension == "xpm" || extension == "jpg") {
        // If the file is an image, load it and create a new QListWidgetItem with the image and time
        QImage image(filePath);
        if (image.isNull()) {
            // If the image cannot be loaded, show an error message and return
            QMessageBox::critical(this, tr("Error"), tr("Cannot load image file: %1").arg(filePath));
            return;
        }

        QTime time = QTime::currentTime();
        QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
        QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)), QString("[%1] photo ").arg(timeString), ui->listWidget_2);
        item->setBackgroundColor(Qt::white);
    } else if (extension == "mp4") {
        // If the file is a video, create a new QListWidgetItem with the video file path and time
        QTime time = QTime::currentTime();
        QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
        QListWidgetItem* item = new QListWidgetItem(QString("[%1] video: %2").arg(timeString).arg(filePath), ui->listWidget_2);
        item->setBackgroundColor(Qt::white);
    } else {
        // If the file is not an image or a video, show an error message and return
        QMessageBox::critical(this, tr("Error"), tr("Invalid file type: %1").arg(filePath));
        return;
    }

    // Scroll to the bottom of listWidget2
    ui->listWidget_2->scrollToBottom();
}*/





void chat::on_listWidget_2_itemClicked(QListWidgetItem *item)
{

        // Check if the clicked item has an icon (i.e., contains a photo)
        if (item->icon().isNull()) {
            // If the clicked item does not contain a photo, return without doing anything
            return;
        }

        // Get the photo from the item's icon
        QPixmap photo = item->icon().pixmap(item->icon().availableSizes().first());

        // Create a message box to display the photo
        QMessageBox msgBox;
        msgBox.setIconPixmap(photo);
        msgBox.setWindowTitle("Photo");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

}








/*void chat::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    // Check if the clicked item contains a photo or a video
    if (item->icon().isNull()) {
        // If the clicked item does not contain a photo, check if it contains a video
        QString text = item->text();
        if (text.startsWith("[") && text.contains("] video: ")) {
            // If the clicked item contains a video, get the file path from its text
            QString filePath = text.mid(text.indexOf(": ") + 2);

            // Open the video file using the default application associated with the file type
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    } else {
        // If the clicked item contains a photo, display it in a message box
        QPixmap photo = item->icon().pixmap(item->icon().availableSizes().first());
        QMessageBox msgBox;
        msgBox.setIconPixmap(photo);
        msgBox.setWindowTitle("Photo");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}*/
