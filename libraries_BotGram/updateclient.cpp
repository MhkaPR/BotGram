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
