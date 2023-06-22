#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <libraries_BotGram/messagewidget.h>
#include <QScrollArea>
#include <QScrollBar>

namespace Ui {
class chatPage;
}

class chatPage : public QWidget
{
    Q_OBJECT

public:
    explicit chatPage(QWidget *parent = nullptr);
    ~chatPage();

    void addMessage(messageWidget* newMsg);

private:
    Ui::chatPage *ui;
    QWidget *ScrollWidget=nullptr;
    QVBoxLayout *ScrollLayout=nullptr;
};

#endif // CHATPAGE_H
