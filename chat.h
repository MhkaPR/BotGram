#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QListWidget>
#include<QtNetwork>
#include<QHostAddress>
#include<QContextMenuEvent>
#include"libraries_BotGram/textmessage.h"
#include <QMap>
#include "Forms/chatpage.h"
#include <libraries_BotGram/userboxwidget.h>
#include <iostream>



namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

signals:
    void fileEnded();
public:
    explicit chat(QWidget *parent = nullptr);
    TextMessage mesg;
      QStringList downloadedFiles;
    ~chat();
   // QString namechat
      void sendApplyForDownload(QString filename);
      void OpenchatPage();
      void SeenNewMessagesInDataBase(QString name);
      int countOfunreadMessages(QString name);
private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_send_message_clicked();

    void on_photo_button_clicked();
     //void on_receiveButton_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);
   // void keyPressEvent(QKeyEvent *event);
   // void keyReleaseEvent(QKeyEvent *event);

    void on_message_text_textChanged();
   void connectToServer();
    void onConnected();
    void onDisconnected();
    void onReadyRead();

    void on_listWidget_2_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_camera_clicked();


    void on_pushButton_voice_clicked();

    void on_pushButton_clicked();

   QString getTweLine(QString data,int lengthOfeachLine);






   void on_commandLinkButton_clicked();

   void addMessage(messageWidget *msg);
   void addMessage(FileMessageWidget *fmsg);
   void addUserBox(UserBoxWidget *fmsg);





   void on_txt_searchUser_textChanged(const QString &arg1);

private:
   void sendmessage(QString message);
    Ui::chat *ui;
     QByteArray name;
  QTcpSocket *socket;
  QSqlDatabase db;
  QString selectedpvname;

  QString pathImgg;
  QPushButton *recievebtn = nullptr;
  QMap<QString,QString> myinformation;
  QMap<QString,QString> usernames_names;
  QMap<QString,QString> usernames_email;
  chatPage *ch=nullptr;
  QListWidgetItem *LastItemChoosed = nullptr;
  QList<QListWidgetItem*> itemsOfListWidgetUsers;

};

#endif // CHAT_H
