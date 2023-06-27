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
#include <QMouseEvent>
#include <QMenu>
#include <QClipboard>
#include <QApplication>


class messageWidget:public QWidget
{
    Q_OBJECT
public:
    explicit messageWidget(const QString& text,QString time, QWidget* parent = nullptr,bool IsSentMessage = false);
    QLabel* m_textLabel=nullptr;
    QLabel* m_timeLabel=nullptr;

    static QString wordWrap(QString inputText,int maxWidth);

    virtual void setBacground_color(bool value);
    virtual void setBacground_color(QColor color);
    virtual void setAlignmentOfmessage(QVBoxLayout *layout,bool value);
    QWidget *messageLayer=nullptr;
    QVBoxLayout *layout_inside = nullptr;

    void mousePressEvent(QMouseEvent *event) override;

protected:
    QColor background_color;


};

#endif // MESSAGEWIDGET_H
