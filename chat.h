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

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr);
    TextMessage mesg;
      QStringList downloadedFiles;
    ~chat();
   // QString namechat
      void sendApplyForDownload(QString filename);
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
  QMap<QString,QMap<QString,QString>> usersinformation;
  chatPage *ch=nullptr;

};

#endif // CHAT_H
