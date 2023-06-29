#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QObject>

#include "database.h"
#include "package.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "userboxwidget.h"
#include "messagewidget.h"
#include "filemessagewidget.h"
#include <QDateTime>
#include <QMap>


class updateClient : public DataBase,public package
{
public:
    virtual void deserialize(QByteArray buffer);
    virtual QByteArray serialize();

    updateClient(QSqlDatabase *DB = nullptr);

    QByteArray getDocJson();
    void setDocJson(QByteArray doc);
    void fixUpdates(QString username,QString RoomName);

    void updateMessages(QSqlDatabase db,QListWidget *userBoxsListWidget,const QMap<QString,QString>& username_name,QString myUsername);

    int IsApply = 0;

private:

    QSqlDatabase *db = nullptr;
    QByteArray docJsonByteArray;

};

#endif // UPDATECLIENT_H
