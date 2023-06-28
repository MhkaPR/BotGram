#ifndef FILEMESSAGEWIDGET_H
#define FILEMESSAGEWIDGET_H

#include <QObject>
#include <QWidget>
#include "messagewidget.h"
#include "QPushButton"
#include <QFile>


//json
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDesktopServices>

class FileMessageWidget : public messageWidget
{
    Q_OBJECT
public:
    QLabel *lbl_title = nullptr;
    FileMessageWidget(const QString& text,QString time, QWidget* parent,const QString& filename,bool IsSentMessage);
    void setfileField(QString title);
    void setIconOfCircle(bool fixDownloadIcon);
    bool checkDownloaded(const QString& filename);
    void setDownloaded(bool value);
    bool getDownloaded();

signals:
    void downloadFile();
public slots:
    void ActiveBtnToCauseOfFileEnded();
    void btn_file_clicked();

private:
    bool downloaded;
    QWidget *FileDownload_Open_field=nullptr;
    QPushButton *btn_file=nullptr;

};

#endif // FILEMESSAGEWIDGET_H
