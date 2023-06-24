#ifndef FILEMESSAGEWIDGET_H
#define FILEMESSAGEWIDGET_H

#include <QObject>
#include <QWidget>
#include "messagewidget.h"
#include "QPushButton"
#include <QFile>
#include "systemmessagepacket.h"
#include "chat.h"

//json
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDesktopServices>

class FileMessageWidget : public messageWidget
{
public:
    FileMessageWidget(const QString& text,QString time, QWidget* parent,const QString& filename,bool IsSentMessage);
    void setfileField(QString title);
    void setIconOfCircle(bool fixDownloadIcon);
    bool checkDownloaded(const QString& filename);

signals:
    void downloadFile(QString filename);
public slots:
    void btn_file_clicked();

private:
    bool downloaded;
    QWidget *FileDownload_Open_field=nullptr;
    QPushButton *btn_file=nullptr;
    QLabel *lbl_title = nullptr;
};

#endif // FILEMESSAGEWIDGET_H
