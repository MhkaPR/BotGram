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

private:
    Ui::chat *ui;

};

#endif // CHAT_H
