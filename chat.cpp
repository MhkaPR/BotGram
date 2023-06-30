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
#include <libraries_BotGram/messagewidget.h>
#include "libraries_BotGram/filemessagewidget.h"
#include <QAbstractItemView>
#include <QWheelEvent>
#include <QGuiApplication>

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
    ui->search_line->hide();
    ui->search_button->hide();
    ui->scrollArea_Emojies->hide();
    ui->btn_Emojies->hide();


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
        usernames_names[query.value("username").toString()] =  query.value("name").toString();
        usernames_email[query.value("username").toString()] =  query.value("email").toString();
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
        QString messageTweLine;
        QString time;
        if(lastUpdateQuery.next())
        {
            time = lastUpdateQuery.value("max(time)").toString();
            time.remove(0,9);

        }

        //ui->listWidget->addItem(name+temp);

        QPixmap f;
        UserBoxWidget *newUser = new UserBoxWidget(f,name,lastUpdateQuery.value("message").toString(),time,0,this);
        int UnreadCount = countOfunreadMessages(newUser->lbl_name.text());
        if(UnreadCount != 0)
            newUser->addUnReadmessageCount(UnreadCount);
        this->addUserBox(newUser);

    }
    query.finish();




    ui->pushButton_send_message->setVisible(false);

    ui->photo_button->setStyleSheet("QPushButton {"
                                    "border: none;"
                                    "background-image: url(:/icons/pin.png);"
                                    "background-position: center;"
                                    "background-color: rgb(255, 255, 127);"

                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: #E6E6E6;"
                                    "}");
    ui->pushButton_camera->setStyleSheet("QPushButton {"
                                         "border: none;"
                                         "background-image: url(:/icons/pin.png);"
                                         "background-position: center;"
                                         "background-color: #4D98E8;"

                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: #E6E6E6;"
                                         "}");
    ui->pushButton_voice->setStyleSheet("QPushButton {"
                                        "border: none;"
                                        "background-image: url(:/icons/pin.png);"
                                        "background-position: center;"
                                        "background-color: rgb(255, 85, 255);"

                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: #E6E6E6;"
                                        "}");





    //QScrollBar * scroolBarOfchat = ui->listWidget_2->verticalScrollBar();
    //    scroolBarOfchat->setSingleStep(50);
    //    scroolBarOfchat->setPageStep(1);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &chat::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &chat::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &chat::onReadyRead);



    connectToServer();


    ui->listWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QScrollBar* scrollBar = ui->listWidget_2->verticalScrollBar();
    ui->listWidget_2->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    ui->listWidget_2->setSelectionRectVisible(false);
    scrollBar->setSingleStep(20);  // set the scroll step to 1 pixel
    //    scrollBar->setPageStep(ui->listWidget_2->height());  // set the page step to the height of the list widget
    //      // set the scroll mode to ScrollPerPixel

    //    // Connect the valueChanged signal of the scrollbar to a custom slot
    //    QObject::connect(scrollBar, &QScrollBar::valueChanged, ui->listWidget_2, [=](int value) {
    //        // Adjust the scroll position to scroll one pixel at a time
    //        int delta = value - scrollBar->value();
    //        if (delta > 1) {
    //            scrollBar->setValue(value - 1);
    //        } else if (delta < -1) {
    //            scrollBar->setValue(value + 1);
    //        }
    //    });


    //add emojis to ram
    QFile emojisFile("DataFiles\\Imojis.btg");

    if(!emojisFile.open(QIODevice::ReadOnly))
    {
        ui->btn_Emojies->setVisible(false);
    }
    else
    {
        QString EmojiseData = emojisFile.readAll();
        emojisFile.close();
        Emojies = EmojiseData.split(" ").toSet();

        int columnNum = 8;

        ui->tableWidget_Emojies->setRowCount(Emojies.count()/columnNum);
        ui->tableWidget_Emojies->setColumnCount(columnNum);
        for (int i = 0; i < columnNum; ++i) {
            ui->tableWidget_Emojies->setColumnWidth(i,87);
        }

        int counter_emojies=0;
        int row=0;

        foreach (const QString& emojie, Emojies)
        {

            if(emojie != "")
            {
                counter_emojies++;

                if(counter_emojies==9)
                {
                    row++;
                    counter_emojies=0;
                }

                QTableWidgetItem* item = new QTableWidgetItem(emojie);
                item->setTextAlignment(Qt::AlignCenter);

                ui->tableWidget_Emojies->setItem(row, counter_emojies-1, item);
            }
        }

    }


    //ui->tableWidget_Emojies->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect( ui->tableWidget_Emojies, &QTableWidget::itemClicked, [=]() {
        QList<QTableWidgetItem*> selectedItems = ui->tableWidget_Emojies->selectedItems();
        if (!selectedItems.isEmpty()) {
            QTableWidgetItem* selectedItem = selectedItems.first();
            // Do something with the selected item
            ui->message_text->insertPlainText(selectedItem->text());

        }
    });


    //        QTableWidget* table = new QTableWidget();
    //        table->setRowCount(Emojies.size());
    //        table->setColumnCount(1);
    //        table->setShowGrid(false); // hide the grid lines
    //        table->setSelectionMode(QAbstractItemView::SingleSelection);
    //        table->setHorizontalHeaderLabels({"Emojis"});

    //        int row = 0;
    //        foreach (const QString& emoji, Emojies) {
    //            QTableWidgetItem* item = new QTableWidgetItem(emoji);
    //            item->setTextAlignment(Qt::AlignCenter);
    //            table->setItem(row++, 0, item);
    //        }

    //        QGridLayout* layout = new QGridLayout();
    //        layout->addWidget(table);
    //        ui->scrollArea_Emojies->setLayout(layout);
    //        QTableWidget* table = new QTableWidget(Emojies.size(), 1);
    //        table->setSelectionMode(QAbstractItemView::SingleSelection);
    //        table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //        table->setHorizontalHeaderLabels({"Emojis"});

    //        int row = 0;
    //        foreach (const QString& emoji, Emojies) {
    //            QTableWidgetItem* item = new QTableWidgetItem(emoji);
    //            table->setItem(row++, 0, item);
    //        }

    //        connect(table, &QTableWidget::itemSelectionChanged, [=]() {
    //            QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    //            if (!selectedItems.isEmpty()) {
    //                QTableWidgetItem* selectedItem = selectedItems.first();
    //                qDebug() << "Selected emoji:" << selectedItem->text();
    //            }
    //        });

    //        ui->scrollArea_Emojies->setWidget(table);


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
        qDebug() << "sdfghm,fdsaasdfhjkkldsdfhk";
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
    connectToServer();
}
//
void chat::onReadyRead()
{
    TextMessage msg;
    QByteArray data = socket->readAll();

    // Handle incoming data from the server


    qDebug() << data[0]<< "//";

    if(data[0] == '~')
    {
        return;
    }
    else if(data[0] == '#')
    {
        updateClient ApplyUpdates;
        ApplyUpdates.IsApply = 1;
        QByteArray bufferApplyUpdate;
        QDataStream outApplying(&bufferApplyUpdate,QIODevice::WriteOnly);
        outApplying.setVersion(QDataStream::Qt_4_0);
        outApplying<<static_cast<short>(ApplyUpdates.getheader())<<ApplyUpdates.serialize();
        socket->write(bufferApplyUpdate);
        socket->waitForBytesWritten();

        qDebug() << "sent apply!";
        //return;

    }
    else
    {


        QDataStream in(&data,QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_0);

        short header;
        in>>header;
        qDebug() << "code of package: " << header;
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

                usernames_names[searching.username] = searching.name;
                usernames_email[searching.email] = searching.email;


                query.clear();

                query.prepare("CREATE TABLE "+searching.name+" (message TEXT , time TEXT,sender INTEGER,isfile INTEGER,Seen INTEGER)");
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




        }
        case package::SYSTEM:
        {


            break;
        }
        case package::UPDATE_CLIENT:
        {

            updateClient ReceiveUpdates;
            ReceiveUpdates.deserialize(data);



            //sendmessage(ReceiveUpdates.getDocJson());
            // process on updates
            ReceiveUpdates.updateMessages(db,ui->listWidget,usernames_names,myinformation["username"]);



            systemMessagePacket updateInServer;
            updateInServer.setSysmsg(package::SysCodes::update_In_last_in_Rooms);

            QByteArray updateBuf;
            QDataStream outUpdateInserver(&updateBuf,QIODevice::WriteOnly);
            outUpdateInserver.setVersion(QDataStream::Qt_4_0);

            outUpdateInserver << static_cast<short>(updateInServer.getheader()) << updateInServer.serialize();

            socket->write(updateBuf);
            socket->waitForBytesWritten();
            break;
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

            //recievebtn->setText(QString::number(bufsize/1024/1024.0));
            //check if end of file
            if(fmsg.IsEndFile())
            {
                //recievebtn->setEnabled(true);
                //recievebtn->setText("Open");
                emit fileEnded();
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

            // msg.deserialize(data);
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonArray messagesArray = doc.array();

            int messagesCount = messagesArray.size();
            for (int i= 0;i< messagesCount;i++)
            {

                QJsonObject obj = messagesArray[i].toObject();
                msg.sender = obj["username"].toString();
                msg.Message = obj["message"].toString();
                msg.timeSend = QDateTime::fromString(obj["date"].toString(),"yyyy.MM.dd-hh:mm:ss.zzz");
                msg.IsFile = obj["isFile"].toBool();

                qDebug() << msg.sender << msg.Message << msg.timeSend << obj["date"];
                //messageWidget *ClinetMessage = new messageWidget(msg.Message,msg.timeSend.toString("hh:mm:ss"),this,false);
                //ch->addMessage(ClinetMessage);
                //this->addMessage(ClinetMessage);



                QSqlQuery query(db);
                sendmessage(usernames_names[msg.sender]);

                query.prepare("INSERT INTO "+usernames_names[msg.sender]+" (message, time,sender,isfile,Seen) VALUES (:message, :time,:sender,:isfile,:Seen)");
                query.bindValue(":message",msg.Message );

                query.bindValue(":time",msg.timeSend.toString("yyyyMMdd hh:mm:ss") );
                query.bindValue(":sender",0);
                query.bindValue(":isfile",msg.IsFile);

                UserBoxWidget *checkUserForCurrect = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));
                if(checkUserForCurrect->lbl_name.text() != usernames_names[msg.getSender()])
                {
                    query.bindValue(":Seen",0);

                    for (int i = 0; i < ui->listWidget->count(); ++i)
                    {
                        UserBoxWidget *checkUser = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
                        if(checkUser->lbl_name.text() == usernames_names[msg.getSender()])
                        {
                            checkUser->addUnReadmessageCount(1);
                            checkUser->lbl_TweLineOfLastMessages.setText(checkUser->getTweLine(msg.getMessage(),50));
                            checkUser->lbl_time.setText(msg.timeSend.toString("hh:mm:ss"));
                            break;
                        }
                    }

                }
                else
                {
                    if(msg.IsFile)
                    {
                        FileMessageWidget *ReceivedFileMessage = new FileMessageWidget("",msg.gettimeSend().toString("hh:mm:ss"),this,msg.getMessage(),false);
                        this->addMessage(ReceivedFileMessage);
                        checkUserForCurrect->lbl_TweLineOfLastMessages.setText(checkUserForCurrect->getTweLine(ReceivedFileMessage->lbl_title->text(),50));
                        checkUserForCurrect->lbl_time.setText(ReceivedFileMessage->m_timeLabel->text());

                    }
                    else
                    {
                        messageWidget *ReceivedMessage = new messageWidget(msg.getMessage(),msg.gettimeSend().toString("hh:mm:ss"),this,false);
                        this->addMessage(ReceivedMessage);
                        checkUserForCurrect->lbl_TweLineOfLastMessages.setText(checkUserForCurrect->getTweLine(ReceivedMessage->m_textLabel->text(),50));
                        checkUserForCurrect->lbl_time.setText(ReceivedMessage->m_timeLabel->text());

                    }
                    ui->listWidget_2->scrollToBottom();
                }

                query.bindValue(":Seen",msg.IsFile);


                // Execute the query
                if (!query.exec()) {
                    QMessageBox::information(this,"warning","Execute the query","ok");

                    return;
                }
                query.finish();

            }



            break;
        }
        }
    }
    // Process the data
}
chat::~chat()
{
    delete ui;

    QString pythonScript = "freeMemory.py";

    QProcess process;
    process.start("python", QStringList() << pythonScript);

    process.waitForFinished();
    qDebug() << "Memory Free!";

    delete this;
}

void chat::sendApplyForDownload(QString filename)
{
    systemMessagePacket msgpacket;
    msgpacket.setSysmsg(package::SysCodes::send_file);
    QJsonObject data;
    data["sender"] = TokenME;
    data["FileName"] = filename;
    UserBoxWidget *currentuserBoxForSendAppayForDownload = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));

    data["room"] = "pv_"+myinformation["username"]+"_"+usernames_names.key(currentuserBoxForSendAppayForDownload->lbl_name.text());
    msgpacket.setinformation(QJsonDocument(data).toJson());

    QByteArray msgbytearray;
    QDataStream out2(&msgbytearray, QIODevice::WriteOnly);
    out2.setVersion(QDataStream::Qt_4_0);
    out2 << static_cast<short>(msgpacket.getheader()) << msgpacket.serialize();
    //    sendmessage(filename);
    socket->write(msgbytearray);

    socket->waitForBytesWritten();
    //    socket->waitForReadyRead();

}

void chat::OpenchatPage()
{

    //    ui->chatPage_Widget->layout()->removeWidget(ch);
    //    ch->deleteLater();
    //    delete ch;
    //    ch = nullptr;
    //    ch = new chatPage;
    //    ui->chatPage_Widget->layout()->addWidget(ch);
    //    ui->chatPage_Widget->setCurrentWidget(ch);
    //    //ch->show();
}

void chat::SeenNewMessagesInDataBase(QString name)
{
    QSqlQuery query(db);
    query.prepare("UPDATE "+name+" SET Seen = 1 WHERE Seen = 0 ");
    if(!query.exec())
    {
        qDebug() << "Failed to execute query in Seen mesages!";
        return;
    }
    query.finish();

}

int chat::countOfunreadMessages(QString name)
{

    QSqlQuery query(db);
    query.prepare("SELECT count(*) FROM  "+name+" WHERE Seen = 0");
    if(!query.exec())
    {
        qDebug() << "Failed to execute query in Seen mesages!";
        exit(1);
    }
    if(query.next())
    {

        int ans = query.value("count(*)").toInt();
        query.finish();
        return ans;

    }
    else{
        qDebug() << "Failed to Find query!!!!";
        query.finish();
        exit(1);
    }
}


void chat::on_listWidget_itemClicked(QListWidgetItem *item)
{

    if(LastItemChoosed != item)
    {


        ui->listWidget_2->clear();
        LastItemChoosed = item;
        // open new chat Page For Another Member
        //OpenchatPage();

        UserBoxWidget *selectedUser=dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(item));
        selectedUser->lbl_UnreadMessagesCount.setText("0");
        selectedUser->lbl_UnreadMessagesCount.setVisible(false);
        SeenNewMessagesInDataBase(selectedUser->lbl_name.text());




        //selectedpvname = item->text().split("\n")[0];
        selectedpvname = selectedUser->lbl_name.text();
        ui->label_onoroff->setText(selectedpvname);

        //QMessageBox::information(this,"sff",itemname);

        QSqlQuery query(db);
        query.prepare("SELECT * FROM  "+selectedpvname+ " ORDER BY time ASC" );

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
            bool isMe = query.value("sender").toBool();



            if(query.value("isfile").toBool()==1)
            {
                FileMessageWidget *newFile_Before = new FileMessageWidget("",time,this,message,isMe);

                this->addMessage(newFile_Before);
                itemsOfListWidgetUsers.append(ui->listWidget->item(ui->listWidget->count()-1));

                //ch->addMessage(newFile_Before);

                connect(newFile_Before,&FileMessageWidget::downloadFile,[=](){

                    this->sendApplyForDownload(message);
                });
                connect(this,&chat::fileEnded,newFile_Before,&FileMessageWidget::ActiveBtnToCauseOfFileEnded);
            }
            else
            {
                messageWidget *messageOfBefore =new messageWidget(message,time,this,isMe);
                //ch->addMessage(messageOfBefore);
                this->addMessage(messageOfBefore);
            }

            //delete  item;
        }

        ui->listWidget_2->scrollToBottom();
        ui->message_text->show();
        ui->pushButton_send_message->show();
        if(ui->message_text->toPlainText().isEmpty())
            ui->pushButton_send_message->setVisible(false);
        //ui->label_selectchat->hide();
        ui->label->hide();
        ui->listWidget_2->show();
        ui->label_selectchat->show();
        ui->photo_button->show();
        ui->pushButton_camera->show();
        ui->pushButton_voice->show();
        ui->label_onoroff->show();
        ui->search_button->show();
        ui->btn_Emojies->show();
        ui->scrollArea_Emojies->hide();
        ui->btn_Emojies->setText("Emojies");

        //ui->label_selectchat->setText(name);
        //ui->label_selectchat->setAlignment(Qt::AlignHCenter);
        ui->label_selectchat->setText(name);
        ui->label_selectchat->setAlignment(Qt::AlignHCenter);
        ui->listWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    }

}


void chat::on_pushButton_send_message_clicked()
{



    QString messageText = ui->message_text->toPlainText();


    QDateTime time = QDateTime::currentDateTime();
    QString timeString = time.toString("hh:mm:ss");
    // or any other time format you prefer
    messageWidget *newMessage = new messageWidget(messageText,timeString,this,true);
    qDebug() << messageText;

    // send to server, message

    UserBoxWidget *currentuserBoxForsendDAtaToServer = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));
    QString receiveOFmessage = usernames_names.key(currentuserBoxForsendDAtaToServer->lbl_name.text());
    /*= usernames_names.key(ui->listWidget->currentItem()->text().split("\n")[0]);*/
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


        //add message
        //ch->addMessage(newMessage);
        this->addMessage(newMessage);
        ui->listWidget_2->scrollToBottom();
        //        QListWidgetItem * newMessageItem = new QListWidgetItem(ui->listWidget_2);

        //        newMessageItem->setSizeHint(newMessage->sizeHint());
        //        ui->listWidget_2->setItemWidget(newMessageItem,newMessage);

        //ch->fixScrollInWidget(newMessage);
        //ui->listWidget_2->addItem(QString("[%1]:\n%2").arg(timeString,messageText));



        // ui->listWidget_2->addItem(item);
        //ui->listWidget_2->scrollToBottom();


        //QString messageTweLine = getTweLine(messages.Message,50);

        //QListWidgetItem *newListWidget = new QListWidgetItem;
        //        QLabel *lbl_message = new QLabel;
        //        lbl_message->setFixedWidth(300);
        //        lbl_message->setText(messages.Message);

        //QString button_message = QString("%1\n%2").arg(messageTweLine,timeString);
        // QListWidgetItem* newItem = new QListWidgetItem();
        // newItem->setTextColor(Qt::black); // set text color to black
        //ui->listWidget->addItem(newItem);
        UserBoxWidget *currentuserBox = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));

        currentuserBox->lbl_TweLineOfLastMessages.setText(currentuserBox->getTweLine(messageText,50));
        //currentuserBox->.setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        currentuserBox->lbl_time.setText(timeString);
        //ui->listWidget->currentItem()->setText(ui->listWidget->currentItem()->text().split("\n")[0]+ "\n"+button_message);

        // Scroll to the bottom of the list widget_2
        //ui->listWidget_2->scrollToBottom();


        //save in database

        QSqlQuery query(db);
        query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile,Seen) VALUES (:message, :time,:sender,:isfile,:Seen)");
        query.bindValue(":message",messages.Message );
        query.bindValue(":time",messages.timeSend.toString("yyyyMMdd hh:mm:ss") );
        query.bindValue(":sender",1);
        query.bindValue(":isfile",0);
        query.bindValue(":Seen",1);


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





    //        //// Create a new QListWidgetItem with the message and time
    //        QDateTime time = QDateTime::currentDateTime();
    //        QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer


    //        // QListWidgetItem* item = new QListWidgetItem(messageWithTime);




    //        // send to server, message
    //        QString receiveOFmessage = usersinformation[ui->listWidget->currentItem()->text().split("\n")[0]]["username"];
    //        QString senderOFmessage = myinformation["username"];
    //        TextMessage messages;
    //        messages.setSender(TokenME);
    //        messages.setReceiver("pv_"+senderOFmessage+"_"+receiveOFmessage);
    //        messages.Message = ui->message_text->toPlainText();
    //        messages.timeSend = messages.gettimeSend().currentDateTime();
    //        messages.stateMessage = package::sendMode;
    //        QByteArray buff2;
    //        QDataStream out2(&buff2,QIODevice::WriteOnly);
    //        out2.setVersion(QDataStream::Qt_4_0);
    //        out2<<static_cast<short>(messages.getheader())<<messages.serialize();
    //        socket->write(buff2);
    //        if(socket->waitForBytesWritten())
    //        {

    //            // Add the item to listWidget2
    //            ui->message_text->clear();



    //            ui->listWidget_2->addItem(QString("[%1]:\n%2").arg(timeString,messageText));



    //            // ui->listWidget_2->addItem(item);
    //            ui->listWidget_2->scrollToBottom();


    //            QString messageTweLine = getTweLine(messages.Message,50);

    //            QListWidgetItem *newListWidget = new QListWidgetItem;
    //            QLabel *lbl_message = new QLabel;
    //            lbl_message->setFixedWidth(300);
    //            lbl_message->setText(messages.Message);

    //            QString button_message = QString("%1\n%2").arg(messageTweLine,timeString);
    //            // QListWidgetItem* newItem = new QListWidgetItem();
    //            // newItem->setTextColor(Qt::black); // set text color to black
    //            //ui->listWidget->addItem(newItem);
    //            ui->listWidget->currentItem()->setText(ui->listWidget->currentItem()->text().split("\n")[0]+ "\n"+button_message);

    //            // Scroll to the bottom of the list widget_2
    //            ui->listWidget_2->scrollToBottom();


    //            //save in database

    //            QSqlQuery query(db);
    //            query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile) VALUES (:message, :time,:sender,:isfile)");
    //            query.bindValue(":message",messages.Message );
    //            query.bindValue(":time",messages.timeSend.toString("yyyyMMdd hh:mm:ss") );
    //            query.bindValue(":sender",1);
    //            query.bindValue(":isfile",0);

    //            // Execute the query
    //            if (!query.exec()) {
    //                QMessageBox::information(this,"warning",query.lastError().text(),"ok");

    //                return;
    //            }
    //            query.finish();
    //            db.commit();

    //            //            ui->listWidget_2->addItem(newListWidget);
    //            //            ui->listWidget_2->setItemWidget(newListWidget,lbl_message);

    //        }
    //        else
    //        {
    //            ui->label_selectchat->setText("wait for connecting...");
    //        }


    //    }
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

    QDateTime time = QDateTime::currentDateTime();
    QString timeString = time.toString("hh:mm:ss"); // or any other time format you prefer
    QImage f(64, 64, QImage::Format_RGB32);
    f.fill(Qt::gray);



    //send file to server

    fileMessage fmsg(TokenME);

    QFileInfo fileInfo(filePath);
    QString mimeType = fileInfo.suffix();
    mimeType = mimeType.toLower();
    //int count =0;
    fmsg.setFileName(time.toString("yyyyMMddhhmmsszzz")+"."+mimeType);

    UserBoxWidget *currentuserBoxForsendFile = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));

    fmsg.setroom("pv_"+myinformation["username"]+"_"+usernames_names.key(currentuserBoxForsendFile->lbl_name.text()));
    fmsg.settimeSend(time);
    fmsg.setSender(TokenME);

    QString suffix;
    suffix=fileInfo.suffix();
    suffix=suffix.toLower();



    fmsg.setcount_size("0");


    fmsg.sendFile(file,socket);

    //fmsg.getFileName()

    file->copy("files/"+fmsg.getFileName());
    delete file;



    //save in database

    QSqlQuery query(db);
    query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile,Seen) VALUES (:message, :time,:sender,:isfile,:Seen)");
    query.bindValue(":message",fmsg.getFileName());
    query.bindValue(":time",fmsg.gettimeSend().toString("yyyyMMdd hh:mm:ss") );
    query.bindValue(":sender",1);
    query.bindValue(":isfile",1);
    query.bindValue(":Seen",1);

    // Execute the query
    if (!query.exec()) {
        QMessageBox::information(this,"warning",query.lastError().text(),"ok");

        return;
    }
    query.finish();
    db.commit();




    // ui->listWidget_2->scrollToBottom();

    QString filename = fmsg.getFileName();

    FileMessageWidget *newfile = new FileMessageWidget("",timeString,this,fmsg.getFileName(),true);
    //ch->addMessage(newfile);
    this->addMessage(newfile);
    ui->listWidget_2->scrollToBottom();

    connect(newfile,&FileMessageWidget::downloadFile,[=](){
        this->sendApplyForDownload(filename);
    });
    connect(this,&chat::fileEnded,newfile,&FileMessageWidget::ActiveBtnToCauseOfFileEnded);

    QString updateuserBox_str = QString("%1\n%2").arg(fmsg.getFileName(),timeString);
    //ui->listWidget->currentItem()->setText(ui->listWidget->currentItem()->text().split("\n")[0]+ "\n"+updateuserBox_str);
    UserBoxWidget *currentuserBox = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));
    currentuserBox->lbl_TweLineOfLastMessages.setText(currentuserBox->getTweLine(updateuserBox_str,50));
    currentuserBox->lbl_time.setText(timeString);
    // ui->listWidget->scrollToBottom();


}

void chat::on_message_text_textChanged()
{
    // Check if the message text box is empty
    if(ui->message_text->toPlainText().isEmpty())
    {
        ui->pushButton_send_message->setVisible(false);
    }
    else
    {
        ui->pushButton_send_message->setVisible(true);
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




    // connect(this,&FileMessageWidget::downloadFile,chat,&chat::btn_file_clicked);


    QPushButton *fileButton = new QPushButton(downloaded ? "Open" : "Download", this);
    recievebtn=fileButton;
    connect(recievebtn, &QPushButton::clicked, [=]() {
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
            UserBoxWidget *currentuserBoxForReceiveFile = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));

            data["room"] = "pv_"+myinformation["username"]+"_"+usernames_names.key(currentuserBoxForReceiveFile->lbl_name.text());
            msgpacket.setinformation(QJsonDocument(data).toJson());

            QByteArray msgbytearray;
            QDataStream out2(&msgbytearray, QIODevice::WriteOnly);
            out2.setVersion(QDataStream::Qt_4_0);
            out2 << static_cast<short>(msgpacket.getheader()) << msgpacket.serialize();
            sendmessage(filename);
            socket->write(msgbytearray);
            socket->waitForBytesWritten();
            recievebtn->setEnabled(false);
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
            recievebtn->setText("Open");
            //} else {
            //  qDebug() << "Failed to save file:" << filePath;
            //}
            // QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));

        }


    });

    // Add the button to the item in the list
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(recievebtn);
    layout->addStretch();
    item->setSizeHint(widget->sizeHint());
    ui->listWidget_2->setItemWidget(item, widget);
    // QDesktopServices::openUrl()

}
// Declare a member variable to store the file button




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
            QDateTime currenttime;
            currenttime=QDateTime::currentDateTime();
            QString currenttimestr =currenttime.toString("hh:mm:ss");
            QString filename = currenttime.toString("yyyyMMddhhmmsszzz")+".wav";




            //            QImage f(64, 64, QImage::Format_RGB32);
            //            f.fill(Qt::gray);


            //            QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap::fromImage(f)), QString("[%1]\n%2").arg(currenttimestr,filename), ui->listWidget_2);
            //            item->setBackgroundColor(Qt::gray);
            // Set the data for the item to the path of the recorded voice file

            // Set the data for the item to the path of the recorded voice file
            QString voiceFileaAddress = recorder->outputLocation().toLocalFile();


            //            item->setData(Qt::UserRole, voiceFile);

            //            // Add the item to the list widget
            //            ui->listWidget_2->addItem(item);

            // Copy the recorded voice file to a new location
            QString newPath = "files/"+filename;

            if(QFile::copy(voiceFileaAddress, QDir::currentPath()+"/"+newPath)){
                voiceDataFile->setFileName(newPath);
                if(voiceDataFile->open(QIODevice::ReadOnly))
                {
                    fileMessage fmsg(TokenME);
                    QFileInfo fileInfo(voiceDataFile->fileName());
                    QString mimeType = fileInfo.suffix();
                    fmsg.setFileName(currenttime.toString("yyyyMMddhhmmsszzz")+"."+mimeType);
                    UserBoxWidget *currentuserBoxForSendVoiceFile = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));


                    fmsg.setroom("pv_"+myinformation["username"]+"_"+usernames_names.key(currentuserBoxForSendVoiceFile->lbl_name.text()));
                    fmsg.settimeSend(QDateTime::currentDateTime());
                    fmsg.setSender(TokenME);

                    // quint64 bufsize=0;

                    fmsg.setcount_size("0");

                    // qDebug() << file.size();

                    fmsg.sendFile(voiceDataFile,socket);

                    FileMessageWidget *VoiceFile = new FileMessageWidget("",currenttimestr,this,filename,true);
                    //ch->addMessage(VoiceFile);
                    this->addMessage(VoiceFile);

                    UserBoxWidget *currentuserBox = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));
                    currentuserBox->lbl_TweLineOfLastMessages.setText(currentuserBox->getTweLine(fmsg.getFileName(),50));
                    currentuserBox->lbl_time.setText(currenttimestr);

                    ui->listWidget_2->scrollToBottom();


                    connect(VoiceFile,&FileMessageWidget::downloadFile,[=](){
                        this->sendApplyForDownload(filename);
                    });
                    connect(this,&chat::fileEnded,VoiceFile,&FileMessageWidget::ActiveBtnToCauseOfFileEnded);


                    QSqlQuery query(db);
                    query.prepare("INSERT INTO "+selectedpvname+" (message, time,sender,isfile,Seen) VALUES (:message, :time,:sender,:isfile,:Seen)");
                    query.bindValue(":message",fmsg.getFileName());

                    query.bindValue(":time",fmsg.gettimeSend().toString("yyyyMMdd hh:mm:ss") );
                    query.bindValue(":sender",1);
                    query.bindValue(":isfile",1);
                    query.bindValue(":Seen",1);
                    // Execute the query
                    if (!query.exec()) {
                        QMessageBox::information(this,"warning",query.lastError().text(),"ok");

                        return;
                    }
                    query.finish();
                    db.commit();

                    //                    QString updateuserBox_str = QString("%1\n%2").arg(fmsg.getFileName(),currenttimestr);
                    //                    ui->listWidget->currentItem()->setText(ui->listWidget->currentItem()->text().split("\n")[0]+ "\n"+updateuserBox_str);


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

    //    if(ansString > 2*lengthOfeachLine)
    //        ansString.remove(2*lengthOfeachLine,ansString.length()-2*lengthOfeachLine);
    return ansString;
}

void chat::sendmessage(QString message)
{
    QMessageBox::information(this,"info",message);
}


void chat::on_commandLinkButton_clicked()
{
    //    ch = new chatPage;
    //    ui->chatPage_Widget->layout()->addWidget(ch);
    //    ui->chatPage_Widget->setCurrentWidget(ch);
    //    //ch->show();
}

void chat::addMessage(messageWidget *msg)
{
    QListWidgetItem *Item = new QListWidgetItem(ui->listWidget_2);
    Item->setSizeHint(msg->sizeHint());
    ui->listWidget_2->setItemWidget(Item,msg);
}


void chat::addMessage(FileMessageWidget *fmsg)
{
    QListWidgetItem *Item = new QListWidgetItem(ui->listWidget_2);
    Item->setSizeHint(fmsg->sizeHint());
    ui->listWidget_2->setItemWidget(Item,fmsg);
}

void chat::addUserBox(UserBoxWidget *userBox)
{
    QListWidgetItem *Item = new QListWidgetItem(ui->listWidget);
    Item->setSizeHint(userBox->sizeHint());
    ui->listWidget->setItemWidget(Item,userBox);
}


void chat::on_txt_searchUser_textChanged(const QString &arg1)
{
    // Create a regular expression from the search query
    QRegExp regex(arg1, Qt::CaseInsensitive, QRegExp::FixedString);

    // Iterate over the list widget items and show/hide them based on the search query
    for (int i = 0; i < ui->listWidget->count(); i++) {

        QListWidgetItem *item = ui->listWidget->item(i);
        UserBoxWidget *matchUser = dynamic_cast<UserBoxWidget*>(ui->listWidget->itemWidget(item));
        QString nameUser = matchUser->lbl_name.text();
        // Use the regular expression to check if the item text contains the search query
        bool matches = regex.indexIn(nameUser) != -1;

        // Show/hide the item based on whether it matches the search query
        item->setHidden(!matches);
    }
}

void chat::on_btn_Search_clicked()
{


    //    if(!ui->search_line->isHidden())
    //    {
    //        ui->search_button->setText("close");
    //    }
    //    else {
    //        ui->search_button->setText("search");
    //    }
}

void chat::on_search_button_clicked()
{
    if (ui->search_line->isHidden()) {

        ui->search_line->setText("");
        ui->search_line->show();
        ui->search_button->setStyleSheet("border-radius:22px;"
                                         " image: url(:/nextOfClickSearch.png);");

    }
    else
    {
        ui->search_line->hide();
        ui->search_button->setStyleSheet("border-radius:10px;"
                                         " image: url(:/SearchIcon.png);");
    }
}

void chat::on_search_line_textChanged(const QString &arg1)
{
    QRegExp regex(arg1, Qt::CaseInsensitive, QRegExp::FixedString);

    // Iterate over the list widget items and show/hide them based on the search query
    for (int i = 0; i < ui->listWidget_2->count(); i++) {

        QListWidgetItem *item = ui->listWidget_2->item(i);
        messageWidget *matchUser = dynamic_cast<messageWidget*>(ui->listWidget_2->itemWidget(item));
        FileMessageWidget *matchUserfile = dynamic_cast<FileMessageWidget*>(ui->listWidget_2->itemWidget(item));
        QString text ;
        if(matchUserfile==nullptr)
        {

            text = matchUser->m_textLabel->text();
        }
        else
        {
            text = matchUserfile->lbl_title->text();
        }



        // Use the regular expression to check if the item text contains the search query
        bool matches = regex.indexIn(text) != -1;

        // Show/hide the item based on whether it matches the search query
        item->setHidden(!matches);

    }
    ui->listWidget_2->scrollToBottom();
}

void chat::on_btn_Emojies_clicked()
{
    if(ui->btn_Emojies->text() == "Emojies")
    {
        ui->btn_Emojies->setText("close");
        ui->scrollArea_Emojies->show();
    }
    else
    {
        ui->btn_Emojies->setText("Emojies");
        ui->scrollArea_Emojies->hide();
    }
}
