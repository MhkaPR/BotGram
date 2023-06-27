#ifndef USERBOXWIDGET_H
#define USERBOXWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class UserBoxWidget:public QWidget
{
public:
    explicit UserBoxWidget(QPixmap ImageOfProfile,QString Name,QString TweLineOfLastMessage,
                  QString Time, QWidget* parent = nullptr);

    static QString wordWrap(QString inputText,int maxWidth);

private:
    QLabel lbl_name;
    QLabel lbl_time;
    QLabel lbl_TweLineOfLastMessages;
    QLabel lbl_image;
    QVBoxLayout RightLayoutForObjects;
    QHBoxLayout mainLayout;
    QWidget complateRightObjects;
};

#endif // USERBOXWIDGET_H
