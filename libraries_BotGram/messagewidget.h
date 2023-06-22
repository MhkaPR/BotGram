#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QFontMetrics>
#include <QDebug>


class messageWidget:public QWidget
{
public:
    explicit messageWidget(const QString& text,QString time, QWidget* parent = nullptr,bool IsSentMessage = false);
    QLabel* m_textLabel=nullptr;
    QLabel* m_timeLabel=nullptr;

    static QString wordWrap(QString inputText,int maxWidth);

   void setBacground_color(bool value);
   void setBacground_color(QColor color);
   void setAlignmentOfmessage(QVBoxLayout *layout,bool value);

private:
    QColor background_color;

};

#endif // MESSAGEWIDGET_H
