#include "updateclient.h"

void updateClient::deserialize(QByteArray buffer)
{
    QDataStream in(&buffer,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    qint64 headerMe;

    in >> headerMe >>IsApply >> docJsonByteArray;

    header =static_cast<HEADERS>(headerMe);


}

QByteArray updateClient::serialize()
{
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << static_cast<short>(header) << IsApply<< docJsonByteArray;
    return  buf;
}

updateClient::updateClient(QSqlDatabase *DB) : db(DB)
{
    header = UPDATE_CLIENT;

}


QByteArray updateClient::getDocJson()
{
    return docJsonByteArray;

}

void updateClient::setDocJson(QByteArray doc)
{
    docJsonByteArray = doc;
}

void updateClient::fixUpdates(QString username, QString RoomName)
{

    //    QJsonObject obj_Rooms;

    //    QSqlQuery QueryInUserInformationForCheckLastMessage(db);
    //    QueryInUserInformationForCheckLastMessage.prepare("SELECT * FROM "+username);
    //    if(!QueryInUserInformationForCheckLastMessage.exec())
    //    {
    //        qDebug() << "error in fixUpdate in User information: ->> "+QueryInUserInformationForCheckLastMessage.lastError().text();
    //        exit(1);
    //    }
    //    while (QueryInUserInformationForCheckLastMessage.next())
    //    {
    //        QString lastTimeInUserInformation = QueryInUserInformationForCheckLastMessage.value("lastMessage_Info").toString();
    //        QString Room = QueryInUserInformationForCheckLastMessage.value("Rooms").toString();


    //        if(lastTimeInUserInformation.isEmpty())
    //            lastTimeInUserInformation = "0";

    //        QSqlQuery QueryOfRoomForReturnNewInformation(db);
    //        QueryOfRoomForReturnNewInformation.prepare("SELECT * FROM "+Room+" WHERE date > :time");
    //        QueryOfRoomForReturnNewInformation.bindValue(":time",lastTimeInUserInformation);
    //        if(!QueryOfRoomForReturnNewInformation.exec())
    //        {
    //            qDebug() << "error in Room data: ->> "+QueryOfRoomForReturnNewInformation.lastError().text();
    //            exit(1);
    //        }


    //        QJsonArray Array_messages;
    //        while (QueryOfRoomForReturnNewInformation.next()) {

    //            QString  dateTime = QueryOfRoomForReturnNewInformation.value("date").toString();
    //            bool isfile = QueryOfRoomForReturnNewInformation.value("isFile").toBool();
    //            QString message = QueryOfRoomForReturnNewInformation.value("message").toString();
    //            QString usernameOfSender = QueryOfRoomForReturnNewInformation.value("name").toString();


    //            QJsonObject obj;
    //            obj.insert("username",usernameOfSender);
    //            obj.insert("message",message);
    //            obj.insert("date",dateTime);
    //            obj.insert("isFile",isfile);

    //            Array_messages.append(obj);



    //        }
    //        obj_Rooms.insert(Room,Array_messages);

    //    }
    //    docJson.setObject(obj_Rooms);

}

void updateClient::updateMessages(QSqlDatabase db, QListWidget *userBoxsListWidget,const QMap<QString,QString>& username_name,QString myUsername)
{
    // convert data to json
    QJsonDocument docData=QJsonDocument::fromJson(docJsonByteArray);
    QJsonObject obj_Rooms = docData.object();
    if(obj_Rooms.isEmpty()) return;
    else
    {
        QMap<QString,UserBoxWidget*> UserBoxes;
        //get userBoxes from userBoxesListwidget
        for (int i = 0; i < userBoxsListWidget->count(); ++i)
        {
            UserBoxWidget *newUserBox = dynamic_cast<UserBoxWidget*>(userBoxsListWidget->itemWidget(userBoxsListWidget->item(i)));
            UserBoxes.insert(newUserBox->lbl_name.text(),newUserBox);
        }
        QStringList objKays = obj_Rooms.keys();
        for (int i = 0; i < objKays.count(); ++i)
        {
            QString RoomName = objKays[i];
            QJsonArray listOfmessages = obj_Rooms[RoomName].toArray();
            if(listOfmessages.isEmpty()) continue;
            else
            {

                int countOfUnSeenMessages = 0;
                QString RoomTableName;
                for (int j = 0; j < listOfmessages.count(); ++j)
                {
                    QJsonObject messageObject = listOfmessages[j].toObject();
                    // get data from each message
                    QString dateM = (QDateTime::fromString(messageObject["date"].toString(),"yyyy.MM.dd-hh:mm:ss.zzz")).toString("yyyyMMdd hh:mm:ss");
                    bool isFileM = messageObject["isFile"].toBool();
                    QString messageM = messageObject["message"].toString();
                    QString usernameM = messageObject["username"].toString();


                    //handle when sender of message is myself or not
                    if(usernameM == myUsername)
                    {
                        QStringList temp = RoomName.split("_");
                        if(temp[0] == "pv")
                        {
                            if(temp[1] == myUsername)
                                RoomTableName = username_name[temp[2]];
                            else
                                RoomTableName = username_name[temp[1]];
                        }
                    }
                    else RoomTableName = username_name[usernameM];

                    QSqlQuery queryOfLastupdateInLocal(db);

                    //1- first get last time that a message sent of received = A

                    queryOfLastupdateInLocal.prepare("SELECT max(time) FROM "+RoomTableName);
                    if(!queryOfLastupdateInLocal.exec())
                    {
                        qDebug() << "query Of Last update In Local --> "+queryOfLastupdateInLocal.lastError().text();
                        exit(1);
                    }
                    if(queryOfLastupdateInLocal.next())
                    {
                        QString maxTime = queryOfLastupdateInLocal.value("max(time)").toString();
                        queryOfLastupdateInLocal.finish();

                        //2- check A is bigger than a message that we want to add or not
                        int Seen;
                        if(maxTime > dateM) Seen = 1;
                        else
                        {
                            Seen =0;
                            countOfUnSeenMessages++;
                        }


                        //save in database
                        QSqlQuery querySaveNewMessges(db);
                        querySaveNewMessges.prepare("INSERT INTO "+RoomTableName+" (message,time,sender,isfile,Seen) VALUES(:m,:t,:s,:i,:S)");

                        querySaveNewMessges.bindValue(":m",messageM);
                        querySaveNewMessges.bindValue(":t",dateM);

                        bool senderIsMySelf;
                        if(usernameM == myUsername)
                        {

                            senderIsMySelf = true;
                            querySaveNewMessges.bindValue(":s",1);
                        }
                        else
                        {
                            senderIsMySelf = false;
                            querySaveNewMessges.bindValue(":s",0);
                        }

                        querySaveNewMessges.bindValue(":i",isFileM);
                        querySaveNewMessges.bindValue(":S",Seen);

                        if(!querySaveNewMessges.exec())
                        {
                            qDebug() << "query Of insert into myDataBase --> "+querySaveNewMessges.lastError().text();
                            exit(1);
                        }

                        querySaveNewMessges.finish();


                        //display last message info on userBoxes and display unreaded messages



                    }// end if


                }// end of loop of listOfMessages
                QSqlQuery queryGetLastMessage(db);
                queryGetLastMessage.prepare("SELECT max(time),message FROM "+RoomTableName);
                if(!queryGetLastMessage.exec())
                {
                    qDebug() << "query Of get last message --> "+queryGetLastMessage.lastError().text();
                    exit(1);
                }

                if(queryGetLastMessage.next())
                {
                    QString LastTime = queryGetLastMessage.value("max(time)").toString();

                    UserBoxWidget* user = UserBoxes[RoomTableName];
                    user->addUnReadmessageCount(countOfUnSeenMessages);
                    user->lbl_time.setText((QDateTime::fromString(LastTime,"yyyyMMdd hh:mm:ss")).toString("hh:mm:ss"));
                    user->lbl_TweLineOfLastMessages.setText(user->getTweLine(queryGetLastMessage.value("message").toString(),50));

                    queryGetLastMessage.finish();
                }


            }// end else

        }//end loop of Rooms

    }

}
