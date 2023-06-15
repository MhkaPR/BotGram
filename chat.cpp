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
#include "email_search.h"
#include"libraries_BotGram/searchuser.h"
#include "libraries_BotGram/systemmessagepacket.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
const int SERVER_PO= 9999;
//const QString TokenME = "pAWmUPKB";
static QString TokenME = "mhka1382";




chat::chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat)
{


    // QString message;
    ui->setupUi(this);


    ui->message_text->hide();
    ui->label_onoroff->hide();
    ui->pushButton_send_message->hide();
    ui->listWidget_2->hide();
    ui->label_selectchat->hide();
    ui->photo_button->hide();
    ui->pushButton_camera->hide();
    ui->pushButton_voice->hide();


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("botgramdatabase.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Failed to open database!";
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM userinformation ");

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to execute query!";
        query.finish();

        return;
    }

    // Process the results
    while (query.next()) {


        QString name = query.value("name").toString();
        QMap<QString,QString> tempMAp;
        tempMAp["username"] = query.value("username").toString();
        tempMAp["email"] = query.value("email").toString();
        usersinformation[name] = tempMAp;
        QString temp;



        //get last update message
        QSqlQuery lastUpdateQuery(db);
        lastUpdateQuery.prepare("SELECT max(time),message FROM "+name);

        if(!lastUpdateQuery.exec())
        {
            qDebug() << "Failed to execute query!";
            lastUpdateQuery.finish();

            return;
        }
        if(lastUpdateQuery.next())
        {
            QString time = lastUpdateQuery.value("max(time)").toString();
            time.remove(0,9);
            QString messageTweLine =getTweLine(lastUpdateQuery.value("message").toString(),50);
            //            QString line1 = messageTweLine.remove(0,50);
            //            QString line2 = messageTweLine.remove(0,qMin(50,messageTweLine.length()));
            //            line1.remove("\n");



            temp = QString("%1\n%2").arg(messageTweLine).arg(time);
            temp = "\n" + temp;
        }

        ui->listWidget->addItem(name+temp);
    }
    query.finish();



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
    ui->pushButton_voice->setStyleSheet("QPushButton {"
                                        "border: none;"
                                        "background-image: url(:/icons/pin.png);"
                                        "background-position: center;"
                                        "background-color: rgb(255, 85, 255);"
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

    //    systemMessagePacket sysmsg;
    //    sysmsg.setSysmsg(package::SysCodes::send_file);

    //    QJsonObject obj;

    //    obj.insert("room","pv_testUser_mhka1382");
    //    obj.insert("FileName","20230603174457740.jpg");

    //    QJsonDocument doc;
    //    doc.setObject(obj);

    //    sysmsg.setinformation(doc.toJson());

    //    QByteArray buf123;
    //    QDataStream out123(&buf123,QIODevice::WriteOnly);
    //    out123.setVersion(QDataStream::Qt_4_0);

    //    out123 << static_cast<short>(sysmsg.getheader()) << sysmsg.serialize();

    //    sendmessage("sff");
    //    socket->write(buf123);
    //    socket->waitForBytesWritten();
    //    socket->waitForReadyRead();

}

void chat::connectToServer()
{
    // Connect to the server on localhost


    QSqlQuery query_sendToken(db);
    query_sendToken.prepare("SELECT * FROM myinformation");

    if(!query_sendToken.exec())
    {
        QMessageBox::information(this,"warning","Execute the query","ok");
        return;
    }

    if(query_sendToken.next())
    {
        socket->connectToHost("127.0.0.1", SERVER_PO);

        myinformation["username"] = query_sendToken.value("username").toString();
        myinformation["password"] = query_sendToken.value("password").toString();
        myinformation["email"] = query_sendToken.value("email").toString();
        myinformation["name"] = query_sendToken.value("name").toString();

        connectVerify conn;
        conn.Token = query_sendToken.value("token").toString();
        TokenME = conn.Token;
        QByteArray buf;
        QDataStream out(&buf,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out<<static_cast<short>(conn.getheader())<<conn.serialize();

        socket->write(buf);
        socket->waitForBytesWritten();

    }

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
    //QThread::sleep(0.5);
    connectToServer();
}

void chat::onReadyRead()
{
    TextMessage msg;
    QByteArray data = socket->readAll();

    // Handle incoming data from the server


    if(data[0] == '~')
    {
        return;
    }


    QDataStream in(&data,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    short header;
    in>>header;
    switch (header) {
    case package::SEARCHUSER:
    {
        searchUserPackat searching;
        searching.deserialize(data);
        if(searching.name != "")
        {
            ui->listWidget->addItem(searching.name);
            QSqlQuery query(db);
            query.prepare("INSERT INTO userinformation (username, name,email) VALUES (:username, :name,:email)");
            query.bindValue(":username", searching.username);
            query.bindValue(":name", searching.name);
            query.bindValue(":email",searching.getemail());



            // Execute the query
            if (!query.exec()) {
                QMessageBox::information(this,"warning","Execute the query","ok");

                return;
            }
            query.clear();

            query.prepare("CREATE TABLE "+searching.name+" (message TEXT , time TEXT,sender INTEGER,isfile INTEGER)");
            if(!query.exec())
            {
                qDebug() << "Failed to create table: " << query.lastError().text();
                return;
            }

        }
        else
        {
            QMessageBox::information(this,"warning!","no user with this email found","ok");
        }
        break;

        //send apply ??
        //receive part 1 ??
        //send ok answer ??
        //while receive part i
        // send ok answer





    }
    case package::FILEMESSAGE :
    {

        //deserialize

        fileMessage fmsg("");

        fmsg.deserialize(data);

        static quint64 bufsize=0;
        bufsize += static_cast<quint64>(data.size());



        QDir cur(QDir::current());
        cur.cd("files");




        //send file, part i in hard
        QFile fileReceiive(cur.path()+"/"+fmsg.getFileName());

        //sendmessage(cur.path()+"/"+fmsg.getFileName());

        if(!fileReceiive.open(QIODevice::Append | QIODevice::WriteOnly))
        {
            sendmessage("receive file:"+fileReceiive.errorString());
            exit(1);
        }
        fileReceiive.write(fmsg.getData());
        fileReceiive.close();


        //check if end of file
        if(fmsg.IsEndFile())
        {
            bufsize = 0;
        }


        socket->write("~");
        socket->waitForBytesWritten();
        //sendmessage("hello filemessage");

        //print how much data received
        //fmsg.getFileName().remove(fmsg.getroom()+"---");
        //(ui->listWidget_2->findItems(fmsg.getFileName(),Qt::MatchContains))[0]->setText(QString("received %1 MB").arg(bufsize/1024/1024));


        break;
    }

    default:
    {
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
        QSqlQuery query(db);
        query.prepare("INSERT INTO "+selectedpvname+" (username, name,email) VALUES (:username, :name,:email)");
        query.bindValue(":message",msg.Message );
        query.bindValue(":time",msg.timeSend.toString("yyyyMMdd hh:mm:ss") );
        query.bindValue(":sender",0);
        query.bindValue(":isfile",msg.IsFile);

        // Execute the query
        if (!query.exec()) {
            QMessageBox::information(this,"warning","Execute the query","ok");

            return;
        }
        query.finish();

        break;
    }
    }






    // Process the data
}
chat::~chat()
{
    delete ui;
    delete this;
}


void chat::on_listWidget_itemClicked(QListWidgetItem *item)
{

    ui->listWidget_2->clear();

    for (int i = 0;i < 23;i++) {
        ui->listWidget_2->addItem("");
    }
    selectedpvname = item->text().split("\n")[0];

    //QMessageBox::information(this,"sff",itemname);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM  "+selectedpvname);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Failed to execute query!";
        return;
    }

    // Process the results
    while (query.next()) {
        QString message = query.value("message").toString();
        QString time = query.value("time").toString();
        time = time.remove(0,9);
        QString temp = QString("[%1]").arg(time)+message;
        bool isMe = query.value("sender").toBool();
        QListWidgetItem* item_Room = new QListWidgetItem(temp);
        if(isMe)
        {
            item_Room->setBackgroundColor(Qt::green);
        }
        if(query.value("isfile").toBool()==1)
        {

            QImage f(64, 64, QImage::Format_RGB32);
            f.fill(Qt::gray);



            QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(f)), QString("[%1]\n%2").arg(time,message), ui->listWidget_2);
            item->setBackgroundColor(Qt::gray);
            ui->listWidget_2->addItem(item);
            ui->listWidget_2->scrollToBottom();
        }
        else

        {
            ui->listWidget_2->addItem(item_Room);
            ui->listWidget_2->scrollToBottom();
        }

        //delete  item;
    }

    ui->message_text->show();
    ui->pushButton_send_message->show();
    //ui->label_selectchat->hide();
    ui->label->hide();
    ui->listWidget_2->show();
    ui->label_selectchat->show();
    ui->photo_button->show();
    ui->pushButton_camera->show();
    ui->pushButton_voice->show();
    ui->label_onoroff->show();

    //ui->label_selectchat->setText(name);
    //ui->label_selectchat->setAlignment(Qt::AlignHCenter);
    ui->label_selectchat->setText(name);
    ui->label_selectchat->setAlignment(Qt::AlignHCenter);



}


void chat::on_pushButton_send_message_clicked()
{


    QString messageText = ui->message_text->toPlainText();
    if(messageText.isEmpty())
    {
        ui->message_text->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
    else
    {


        //// Create a new QListWidgetItem with the message and time
        QDateTime time = QDateTime::currentDateTime();
        QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer


        // QListWidgetItem* item = new QListWidgetItem(messageWithTime);




        // send to server, message
        QString receiveOFmessage = usersinformation[ui->listWidget->currentItem()->text().split("\n")[0]]["username"];
        QString senderOFmessage = myinformation["username"];
        TextMessage messages;
        messages.setSender(TokenME);
        messages.setReceiver("pv_"+senderOFmessage+"_"+receiveOFmessage);
        messages.Message = ui->message_text->toPlainText();
        messages.timeSend = messages.gettimeSend().currentDateTime();
        messages.stateMessage = package::sendMode;
        QByteArray buff2;
        QDataStream out2(&buff2,QIODevice::WriteOnly);
        out2.setVersion(QDataStream::Qt_4_0);
        out2<<static_cast<short>(messages.getheader())<<messages.serialize();
        socket->write(buff2);
        if(socket->waitForBytesWritten())
        {

            // Add the item to listWidget2
            ui->message_text->clear();



            ui->listWidget_2->addItem(QString("[%1]:\n%2").arg(timeString,messageText));



            // ui->listWidget_2->addItem(item);
            ui->listWidget_2->scrollToBottom();


            QString messageTweLine = getTweLine(messages.Message,50);

            QListWidgetItem *newListWidget = new QListWidgetItem;
            QLabel *lbl_message = new QLabel;
            lbl_message->setFixedWidth(300);
            lbl_message->setText(messages.Message);

            QString button_message = QString("%1\n%2").arg(messageTweLine,timeString);
            // QListWidgetItem* newItem = new QListWidgetItem();
            // newItem->setTextColor(Qt::black); // set text color to black
            //ui->listWidget->addItem(newItem);
            ui->listWidget->currentItem()->setText(ui->listWidget->currentItem()->text().split("\n")[0]+ "\n"+button_message);

            // Scroll to the bottom of the list widget_2
            ui->listWidget_2->scrollToBottom();


            //save in database

            QSqlQuery query(db);
            query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile) VALUES (:message, :time,:sender,:isfile)");
            query.bindValue(":message",messages.Message );
            query.bindValue(":time",messages.timeSend.toString("yyyyMMdd hh:mm:ss") );
            query.bindValue(":sender",1);
            query.bindValue(":isfile",0);

            // Execute the query
            if (!query.exec()) {
                QMessageBox::information(this,"warning",query.lastError().text(),"ok");

                return;
            }
            query.finish();
            db.commit();

            //            ui->listWidget_2->addItem(newListWidget);
            //            ui->listWidget_2->setItemWidget(newListWidget,lbl_message);

        }
        else
        {
            ui->label_selectchat->setText("wait for connecting...");
        }


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





    fileMessage fmsg(TokenME);

    QFileInfo fileInfo(filePath);
    QString mimeType = fileInfo.suffix();
   mimeType = mimeType.toLower();
            //int count =0;
    fmsg.setFileName(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")+"."+mimeType);
    fmsg.setroom("pv_"+myinformation["username"]+"_"+usersinformation[ui->listWidget->currentItem()->text().split("\n")[0]]["username"]);
    fmsg.settimeSend(QDateTime::currentDateTime());
    fmsg.setSender(TokenME);

     QString suffix;
     suffix=fileInfo.suffix();
     suffix=suffix.toLower();
    QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(f)), QString("[%1]\n%2").arg(timeString,fmsg.gettimeSend().toString("yyyyMMddhhmmsszzz")+"."+suffix), ui->listWidget_2);
    item->setBackgroundColor(Qt::gray);


    quint64 fileSize = static_cast<quint64>(file->size());
    // quint64 bufsize=0;

    fmsg.setcount_size("0");

    // qDebug() << file.size();

     //QMessageBox::information(this,"Dv",TokenME,"Fvdf");
    fmsg.sendFile(file,socket);

    delete file;



    //save in database

    QSqlQuery query(db);
    query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile) VALUES (:message, :time,:sender,:isfile)");
    query.bindValue(":message",fmsg.getFileName());
    query.bindValue(":time",fmsg.gettimeSend().toString("yyyyMMdd hh:mm:ss") );
    query.bindValue(":sender",1);
    query.bindValue(":isfile",1);

    // Execute the query
    if (!query.exec()) {
        QMessageBox::information(this,"warning",query.lastError().text(),"ok");

        return;
    }
    query.finish();
    db.commit();




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


/*void chat::on_listWidget_2_itemClicked(QListWidgetItem *item)
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
    else{
        //        QString voiceFile = item->data(Qt::UserRole).toString();

        //        // Check if the file exists
        //        if(QFile::exists(voiceFile)){
        //            // Create a new instance of QDesktopServices and open the file with the default application
        //            QDesktopServices::openUrl(QUrl::fromLocalFile(voiceFile));
        //        }else{
        //            QMessageBox::warning(this, "Error", "Voice file not found!");
        //        }

        QDir cur = QDir::current();
        cur.cd("files");


        QString filename = item->text();

        filename.remove(0,11);
        systemMessagePacket msgpacket;
        msgpacket.setSysmsg(package::SysCodes::send_file);
        QJsonObject data;
         data["sender"] = TokenME;
         data["FileName"] = filename;
         data["room"]= "pv_testUser_mhka1382";


         // Convert the JSON object to a JSON document
         QJsonDocument json_doc;
         json_doc.setObject(data);

         // Convert the JSON document to a QByteArray and send it as a message
         QByteArray msgbytearray;
         msgbytearray = json_doc.toJson();
         msgpacket.setinformation(msgbytearray);

         QDataStream out2(&msgbytearray,QIODevice::WriteOnly);
         out2.setVersion(QDataStream::Qt_4_0);
         out2<<static_cast<short>(msgpacket.getheader())<<msgpacket.serialize();
         socket->write(msgbytearray);
         socket->waitForBytesWritten();

       QDesktopServices::openUrl(QUrl::fromLocalFile(pathImgg));




    }
}*/







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

    // Get the filename from the clicked item
    QString filename = item->text();
    filename.remove(0,11);

    // Check if the file has already been downloaded
    bool downloaded = downloadedFiles.contains(filename);
    //QMessageBox::information(this,"dff",QString::number(downloaded),"sdd");
    // Create a button to download or open the file, depending on the download status
    QPushButton *fileButton = new QPushButton(downloaded ? "Open" : "Download", this);
    connect(fileButton, &QPushButton::clicked, [=]() {
        ui->listWidget_2->setCurrentItem(item);
        if (downloaded) {
            // Check that the file exists before attempting to open it

          //  Open the downloaded file
            QString filePath = "files/" + filename;
             //QProcess::startDetached("xdg-open", QStringList() << filePath);
           QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        } else {
            // Send a request to the server to receive the file
            systemMessagePacket msgpacket;
            msgpacket.setSysmsg(package::SysCodes::send_file);
            QJsonObject data;
            data["sender"] = TokenME;
            data["FileName"] = filename;
            data["room"] = "pv_"+myinformation["username"]+"_"+usersinformation[ui->listWidget->currentItem()->text().split("\n")[0]]["username"];
            msgpacket.setinformation(QJsonDocument(data).toJson());

            QByteArray msgbytearray;
            QDataStream out2(&msgbytearray, QIODevice::WriteOnly);
            out2.setVersion(QDataStream::Qt_4_0);
            out2 << static_cast<short>(msgpacket.getheader()) << msgpacket.serialize();
            socket->write(msgbytearray);
            socket->waitForBytesWritten();

            // Wait for the file to be received
           // socket->waitForReadyRead();

            // Save the received file to the "files" directory
            //QByteArray dataReceived = socket->readAll();
          //  QString filePath = "files/" + filename;
            //QFile file(filePath);
            //if (file.open(QIODevice::WriteOnly)) {
              //  file.write(dataReceived);
                //file.close();
                // Add the filename to the list of downloaded files
                downloadedFiles.append(filename);
                // Update the button text to "Open"
                fileButton->setText("Open");
            //} else {
              //  qDebug() << "Failed to save file:" << filePath;
            //}
           // QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));

        }


    });

    // Add the button to the item in the list
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(fileButton);
    layout->addStretch();
    item->setSizeHint(widget->sizeHint());
    ui->listWidget_2->setItemWidget(item, widget);
   // QDesktopServices::openUrl()

}
// Declare a member variable to store the file button
QPushButton *fileButton;



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









void chat::on_pushButton_voice_clicked()
{


    QFile *voiceDataFile =new QFile;
    // Create a new instance of QAudioRecorder
    QAudioRecorder *recorder = new QAudioRecorder(this);

    // Create a new instance of QPushButton and set it as the record button for the recorder
    QPushButton *recordButton = new QPushButton("Record", this);
    connect(recordButton, &QPushButton::clicked, recorder, &QAudioRecorder::record);

    // Create a new instance of QPushButton and set it as the stop button for the recorder
    QPushButton *stopButton = new QPushButton("Stop", this);
    connect(stopButton, &QPushButton::clicked, recorder, &QAudioRecorder::stop);

    // Create a new instance of QHBoxLayout and add the record and stop buttons to it
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(recordButton);
    buttonLayout->addWidget(stopButton);

    // Create a new instance of QWidget and set the layout for it
    QWidget *widget = new QWidget(this);
    widget->setLayout(buttonLayout);

    // Create a new instance of QVBoxLayout and add the widget to it
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(widget);

    // Create a new instance of QDialog and set the layout for it
    QDialog *dialog = new QDialog(this);
    dialog->setLayout(layout);

    // Connect the stateChanged signal of the recorder to a lambda function that adds the recorded voice file to the list widget and saves it to a file
    connect(recorder, &QAudioRecorder::stateChanged, this, [=](QAudioRecorder::State state){
        if(state == QAudioRecorder::StoppedState){
            // Create a new instance of QListWidgetItem and set the icon and text for it
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icons/voice_icon.png"), QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            // Set the data for the item to the path of the recorded voice file
            QString voiceFile = recorder->outputLocation().toLocalFile();


            item->setData(Qt::UserRole, voiceFile);

            // Add the item to the list widget
            ui->listWidget_2->addItem(item);

            // Copy the recorded voice file to a new location
            QString newPath = QFileDialog::getSaveFileName(this, "Save Voice", "", "WAV Files (*.wav)");
            if(!newPath.isEmpty()){
                if(QFile::copy(voiceFile, newPath)){
                    voiceDataFile->setFileName(newPath);
                    if(voiceDataFile->open(QIODevice::ReadOnly))
                    {
                        fileMessage fmsg(TokenME);
                        QFileInfo fileInfo(voiceDataFile->fileName());
                        QString mimeType = fileInfo.suffix();
                        fmsg.setFileName(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")+"."+mimeType);
                        fmsg.setroom("pv_"+myinformation["username"]+"_"+usersinformation[ui->listWidget->currentItem()->text().split("\n")[0]]["username"]);
                        fmsg.settimeSend(QDateTime::currentDateTime());
                        fmsg.setSender(TokenME);

                        // quint64 bufsize=0;

                        fmsg.setcount_size("0");

                        // qDebug() << file.size();

                        fmsg.sendFile(voiceDataFile,socket);
                    }
                    else
                    {
                        QMessageBox::information(this,"sdfg",voiceDataFile->errorString());
                    }
                    QMessageBox::information(this, "Success", "Voice saved successfully!");
                }else{
                    QMessageBox::warning(this, "Error", "Failed to save voice!");
                }
            }
        }
    });

    // Set up the recorder and show the dialog
    QString filename = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    filename = QDir::tempPath() + "/" + filename + ".wav";
    recorder->setOutputLocation(QUrl::fromLocalFile(filename));
    // recorder->setAudioSettings(QAudioEncoderSettings(), QAudioFormat::wavformat);
    recorder->setAudioInput("Default");
    recorder->record();

    dialog->show();




}



void chat::on_pushButton_clicked()
{
    email_search *w3 = new email_search(socket);
    w3->setWindowTitle("search page");
    w3->resize(1310,810);
    //this->hide();
    w3->show();
}

QString chat::getTweLine(QString data,int lengthOfeachLine)
{

    int lenData = data.length();

    QString ansString;
    QString line1 = data.left(qMin(lengthOfeachLine,lenData));
    data.remove(0,qMin(lengthOfeachLine,lenData));
    QStringList answer = line1.split("\n");
    // first part of lines
    if(answer.length() > 1)
    {

        return answer[0]+"\n"+answer[1];
    }
    else
    {
        ansString = answer[0];
    }

    // secend line
    QString line2 = data.left(qMin(lengthOfeachLine,lenData));
    data.remove(0,qMin(lengthOfeachLine,lenData));
    answer = line2.split("\n");
    if(answer.length() > 1)
    {

        return answer[0]+"\n"+answer[1];
    }
    else
    {

        ansString += ("\n"  + answer[0]);
    }

    return ansString;
}

void chat::sendmessage(QString message)
{
    QMessageBox::information(this,"info",message);
}

