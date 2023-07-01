#include "filemessagewidget.h"

FileMessageWidget::FileMessageWidget(const QString& text,QString time, QWidget* parent,const QString& filename,bool IsSentMessage):
    messageWidget(text, time, parent, IsSentMessage)
{



    setfileField(filename);
    //    qDebug() << FileDownload_Open_field;
    layout_inside->insertWidget(0,FileDownload_Open_field);




}

void FileMessageWidget::setfileField(QString title)
{
    FileDownload_Open_field = new QWidget(messageLayer);

    QHBoxLayout *Circle_Title_layout = new QHBoxLayout(FileDownload_Open_field);

    btn_file = new QPushButton(FileDownload_Open_field);
    btn_file->setFixedSize(QSize(50,50));
    btn_file->setStyleSheet("border-radius :25px;background-color : #4FC0D0;");
    btn_file->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QPixmap fileIcon;
    if(!checkDownloaded(title))
    {
        fileIcon= QPixmap("DataFiles\\pngwing.png");
        downloaded = false;
    }
    else
    {
        fileIcon= QPixmap("DataFiles\\downloadedFIleIcon.png");
        downloaded = true;
    }
    btn_file->setIcon(QIcon(fileIcon));
    connect(btn_file,&QPushButton::clicked,this,&FileMessageWidget::btn_file_clicked);


    lbl_title = new QLabel(title,FileDownload_Open_field);

    lbl_title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    Circle_Title_layout->addWidget(btn_file);
    Circle_Title_layout->addWidget(lbl_title);



    FileDownload_Open_field->setStyleSheet("border:0px solid #000000;");
    //FileDownload_Open_field->setFixedHeight(75);
    FileDownload_Open_field->setLayout(Circle_Title_layout);
    FileDownload_Open_field->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

}

void FileMessageWidget::setIconOfCircle(bool fixDownloadIcon)
{
    if(fixDownloadIcon)
    {
        // downlaod Icon
        btn_file->setIcon(QIcon(QPixmap("DataFiles\\pngwing.png")));
    }
    else
    {
        //open Icon
        btn_file->setIcon(QIcon(QPixmap("DataFiles\\downloadedFIleIcon.png")));
    }
}

bool FileMessageWidget::checkDownloaded(const QString& filename)
{
    QFile *findFile= new QFile("files/"+filename);
    bool ans = findFile->exists();
    delete findFile;
    if(ans) return true;
    else return  false;


}

void FileMessageWidget::setDownloaded(bool value)
{
    downloaded = value;
}

bool FileMessageWidget::getDownloaded()
{
    return downloaded;
}

void FileMessageWidget::ActiveBtnToCauseOfFileEnded()
{
    btn_file->setEnabled(true);
    btn_file->setText("");
}

void FileMessageWidget::btn_file_clicked()
{
    QString filename = lbl_title->text();
    if(!checkDownloaded(lbl_title->text()))
    {
        if(downloaded == true)
        {
            setIconOfCircle(true);
            downloaded =false;
            qDebug()<<downloaded << 1;
        }
        else
        {
            setIconOfCircle(false);
            downloaded =true;
            btn_file->setEnabled(false);
            btn_file->setText("??");
            qDebug()<<downloaded << 2;
            emit downloadFile();

        }



    }
    else
    {
        QString filePath = "files/" + filename;
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
         qDebug()<<downloaded << 3;

    }

}
