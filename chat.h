#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr);
    ~chat();
   // QString namechat;

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_send_message_clicked();

    void on_photo_button_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);


private:
    Ui::chat *ui;
     QByteArray name;

};

#endif // CHAT_H
