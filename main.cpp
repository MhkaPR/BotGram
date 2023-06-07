#include "botgram.h"
#include"chat.h"

#include <QApplication>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     chat c;
     botgram w;


    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("botgramdatabase.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Failed to open database!";
        return 1;
    }
    QSqlQuery query(db);
    query.prepare("SELECT username,name FROM myinformation WHERE username != NULL AND name != NULL");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return 1;
    }
    if(query.next())
    {
        query.finish();
        db.close();

     c.show();
    }
    else
    {
        query.finish();
        db.close();



        w.show();
    }

    return a.exec();
}
