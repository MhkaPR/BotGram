#include "botgram.h"
#include"chat.h"

#include <QApplication>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSql>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    chat *c=nullptr;
    botgram *w=nullptr;


    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("botgramdatabase.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Failed to open database!";
        return 1;
    }
    QSqlQuery query(db);
    query.prepare("SELECT username,name FROM myinformation WHERE name != '' ;");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return 1;
    }
    if(query.next())
    {
        query.finish();
        db.close();
        c= new chat;

        c->show();
    }
    else
    {
        query.finish();
        db.close();


         w= new botgram;
        w->show();
    }

    return a.exec();
}
