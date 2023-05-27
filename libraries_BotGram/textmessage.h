#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include "database.h"
#include <QDateTime>
#include "package.h"


class TextMessage : public package
{
public:
    TextMessage();
    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();
    QString sender;
    QString Reciever ;
    QString Message;
    QDateTime timeSend;
    SEND_STATE stateMessage;
    QString getSender();
     QString getReciever();
      QString getMessage();
       QDateTime gettimeSend();
       SEND_STATE getstateMessage();
       QByteArray buf;
       bool IsFile;

       void setSender(QString value);
        void setReceiver(QString value);
private:

};

#endif // TEXTMESSAGE_H
