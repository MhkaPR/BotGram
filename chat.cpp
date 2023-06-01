#include "chat.h"
#include "ui_chat.h"
#include<QListWidget>
#include"botgram.h"
#include<QMessageBox>
#include<QPushButton>
#include<QFileDialog>
#include<QDesktopServices>
#include<QHBoxLayout>
#include"libraries_BotGram/connectverify.h"
#include<QtNetwork>
#include<QHostAddress>
#include<QThread>
#include"libraries_BotGram/textmessage.h"
#include<QApplication>
#include<QClipboard>
#include<QMenu>
#include<QAction>
#include<QString>
#include<QContextMenuEvent>
#include<QtMultimedia/QCameraImageCapture>
#include<QtMultimediaWidgets/QCameraViewfinder>
#include<QtMultimedia/QCamera>
#include<QAudioRecorder>
#include<QDir>
#include<QAudioEncoderSettings>
#include"libraries_BotGram/filemessage.h"

const int SERVER_PO= 9999;
//const QString TokenME = "pAWmUPKB";
const QString TokenME = "mhka1382";




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
        ui->pushButton_camera->hide();
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
    ui->pushButton_camera->setStyleSheet("QPushButton {"
                                         "border: none;"
                                         "background-image: url(:/icons/pin.png);"
                                         "background-position: center;"
                                         "background-color: #4D98E8;"
                                         "padding: 0px 10px 0px 10px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: #E6E6E6;"
                                         "}");





    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &chat::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &chat::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &chat::onReadyRead);

    connectToServer();

}

void chat::connectToServer()
{
    // Connect to the server on localhost
    socket->connectToHost("127.0.0.1", SERVER_PO);
    connectVerify conn;
    conn.Token = TokenME;
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out<<static_cast<short>(conn.getheader())<<conn.serialize();
    socket->write(buf);

}

void chat::onConnected()
{
    // Handle the connection to the server
    ui->label_selectchat->setText("Connected to server");
}

void chat::onDisconnected()
{
    // Handle the disconnection from the server
    ui->label_selectchat->setText("wait for connecting...");
    QTcpSocket *socket1 = qobject_cast<QTcpSocket*>(sender());
    if(socket1->error()==QAbstractSocket::RemoteHostClosedError)
    {
        socket1->disconnectFromHost();
    }
    else
    {
        QMessageBox::information(this,"error","socket error:"+socket1->errorString());
    }
    QThread::sleep(0.5);
    connectToServer();
}

void chat::onReadyRead()
{
    TextMessage msg;

    // Handle incoming data from the server
    QByteArray data = socket->readAll();
    if(data[0] == '~')
    {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj;
    obj = doc.object();
    QStringList list;
    list  = obj.keys();
    msg.sender = list[0];

    QJsonArray arr = obj[msg.sender].toArray();
    msg.Message = arr[0].toString();
    msg.timeSend = QDateTime::fromString(arr[1].toString());
    mesg = msg;
    ui->listWidget_2->addItem(mesg.sender+":"+mesg.Message+" // "+arr[1].toString());
    ui->listWidget_2->scrollToBottom();



    // Process the data
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
    ui->pushButton_camera->show();

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


        // Set the background color based on the length of the message
        if (messageLength < 10) {
            item->setBackgroundColor(Qt::green);
        } else {
            item->setBackgroundColor(Qt::green);
        }

        // Add the item to listWidget2
        TextMessage messages;
        messages.setSender(TokenME);
        messages.setReceiver("pv_testUser_mhka1382");
        messages.Message = ui->message_text->toPlainText();
        messages.timeSend = messages.gettimeSend().currentDateTime();
        messages.stateMessage = package::sendMode;
        QByteArray buff2;
        QDataStream out2(&buff2,QIODevice::WriteOnly);
        out2.setVersion(QDataStream::Qt_4_0);
        out2<<static_cast<short>(messages.getheader())<<messages.serialize();
        socket->write(buff2);
        socket->waitForBytesWritten();

        ui->message_text->clear();
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
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose an image file"), QDir::homePath(), tr("All Files (*.*)"));
    if (filePath.isEmpty()) {
        // If the user cancels the file dialog, return without doing anything
        return;
    }

    // Load the image from the file


    QFile *file=new QFile(filePath);


    //    QImage image(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        // If the image cannot be loaded, show an error message and return
        QMessageBox::critical(this, tr("Error"), tr("Cannot load image file: %1").arg(filePath));
        return;
    }

    pathImgg = filePath;
    // Create a new QListWidgetItem with the image and time, and add it to listWidget_2
    QTime time = QTime::currentTime();
    QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
    QImage f(64, 64, QImage::Format_RGB32);
    f.fill(Qt::gray);
    QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(f)), QString("[%1] ").arg(timeString), ui->listWidget_2);
    item->setBackgroundColor(Qt::white);

    fileMessage fmsg(TokenME);
    QFileInfo fileInfo(filePath);
    QString mimeType = fileInfo.suffix();
    int count =0;
    fmsg.setFileName(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")+"."+mimeType);
    fmsg.setroom("pv_testUser_mhka1382");
    fmsg.settimeSend(QDateTime::currentDateTime());
    fmsg.setSender(TokenME);

    quint64 fileSize = static_cast<quint64>(file->size());
    // quint64 bufsize=0;

    fmsg.setcount_size("0");

    // qDebug() << file.size();


    fmsg.sendFile(file,socket);

    file->close();
    delete file;





    ui->listWidget_2->scrollToBottom();

    // Clear all items in listWidget except the first item
    while (ui->listWidget->count() > 1) {
        delete ui->listWidget->takeItem(1);
    }

    // Create a new QListWidgetItem with the text "photo" and the time, and add it to listWidget
    QListWidgetItem* newItem = new QListWidgetItem(QString("[%1] file").arg(timeString), ui->listWidget);
    newItem->setBackgroundColor(Qt::white);

    ui->listWidget->scrollToBottom();


}











void chat::on_message_text_textChanged()
{
    // Check if the message text box is empty
    QString message = ui->message_text->toPlainText().trimmed();
    if (message.isEmpty()) {
        // If the message text box is empty, clear the "typing..." message from the label
        ui->label_selectchat->setText(name);
    } else {
        // If the message text box is not empty, set the "typing..." message in the label
        ui->label_selectchat->setText("typing...              " +  name);
    }
}


void chat::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    if (item->icon().isNull()) {
        // If the clicked item does not contain an icon, it may contain text
        QString text = item->text();
        if (!text.isEmpty()) {
            // Remove the timestamp from the text if it exists
            text.remove(QRegularExpression("^\\[\\d{2}:\\d{2}:\\d{2}\\] "));

            // If the item contains text, open a context menu to copy the text
            QMenu menu(this);
            QAction *copyAction = menu.addAction("Copy");
            connect(copyAction, &QAction::triggered, [=]() {
                QClipboard *clipboard = QGuiApplication::clipboard();
                clipboard->setText(text);
            });
            menu.exec(QCursor::pos());
        }
        return;
    }


    QDesktopServices::openUrl(QUrl::fromLocalFile(pathImgg));

}

void chat::on_listWidget_2_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget_2->itemAt(pos);
    if (item) {
        QMenu menu(this);
        QAction *copyAction = menu.addAction("Copy");
        connect(copyAction, &QAction::triggered, [=]() {
            QString text = item->text();
            if (!text.isEmpty()) {
                QClipboard *clipboard = QGuiApplication::clipboard();
                clipboard->setText(text);
            }
        });
        if (!item->icon().isNull()) {
            QAction *saveAction = menu.addAction("Save Image");
            connect(saveAction, &QAction::triggered, [=]() {
                QPixmap photo = item->icon().pixmap(item->icon().availableSizes().first());
                if (!photo.isNull()) {
                    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
                    if (!fileName.isEmpty()) {
                        photo.save(fileName);
                    }
                }
            });
        }
        menu.exec(ui->listWidget_2->mapToGlobal(pos));
    }
}


void chat::on_pushButton_camera_clicked()
{
    QCamera *camera = new QCamera(this);  // create a new camera instance
    camera->setCaptureMode(QCamera::CaptureStillImage);  // set the capture mode to still image
    camera->start();  // start the camera

    // create a new viewfinder widget to display the camera's output
    QCameraViewfinder *viewfinder = new QCameraViewfinder(this);
    viewfinder->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    viewfinder->setFixedSize(1280, 720);

    // create a new image capture instance
    QCameraImageCapture *imageCapture = new QCameraImageCapture(camera, this);

    // connect the image captured signal to a slot that saves the image and adds it to the list widget
    connect(imageCapture, &QCameraImageCapture::imageCaptured, this, [=](int id, QImage image) {
        // save the image to a file
        QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz") + ".jpg";
        imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
        imageCapture->capture(fileName);

        // create a new QListWidgetItem and add it to listwidget2
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)), fileName, ui->listWidget_2);
        item->setSizeHint(QSize(400, 300));

    });

    // set the camera's viewfinder to the viewfinder widget
    camera->setViewfinder(viewfinder);

    // show the viewfinder widget in a new window
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(viewfinder);
    dialog->setLayout(layout);
    dialog->show();

    // connect the capture button to a slot that captures the image
    QPushButton *captureButton = new QPushButton(tr("Capture"), dialog);
    layout->addWidget(captureButton);
    connect(captureButton, &QPushButton::clicked, this, [=]() {
        imageCapture->capture();
    });
}





void chat::on_pushButton_clicked()
{
    QAudioRecorder *recorder = new QAudioRecorder(this);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Recording"), "untitled", tr("Audio Files (*.mp3)"));

    if (!fileName.isEmpty()) {
        QAudioEncoderSettings settings;
        settings.setCodec("audio/mpeg");
        settings.setQuality(QMultimedia::HighQuality);
        recorder->setEncodingSettings(settings);
        recorder->setOutputLocation(QUrl::fromLocalFile(fileName));
        recorder->record();
    }
}


