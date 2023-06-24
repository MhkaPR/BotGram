#include "filemessagewidget.h"

FileMessageWidget::FileMessageWidget(const QString& text,QString time, QWidget* parent,const QString& filename,bool IsSentMessage):
    messageWidget(text, time, parent, IsSentMessage)
{



    setfileField(filename);
    //    qDebug() << FileDownload_Open_field;
    layout_inside->insertWidget(0,FileDownload_Open_field);



    //    layout_inside->addWidget(m_textLabel);
    //    layout_inside->addWidget(m_timeLabel);

    //    this->setMinimumHeight(100);




    //    setBacground_color(IsSentMessage);
    //    QWidget *messageLayer = new QWidget(this);

    //    messageLayer->setMaximumWidth(parent->geometry().width()/5*4);
    //    messageLayer->setMinimumHeight(100);


    //    // Create the text label
    //    m_textLabel = new QLabel(messageLayer);


    //    m_textLabel->setMaximumWidth(this->maximumWidth());
    //    QFontMetrics fm(m_textLabel->font());
    //    int Ach =fm.horizontalAdvance("W");



    //    //word wrapping
    //    int lenText = text.length();
    //    QString newText=text;
    //    if(lenText >= 100) newText = wordWrap(text,(parent->geometry().width()/5*4)/Ach);
    //    m_textLabel->setWordWrap(true);

    //    //fix message
    //    m_textLabel->setText("");
    //    m_textLabel->setFixedHeight(0);
    //    m_textLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Fixed);
    //    m_textLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignTop);

    //    m_textLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 10px;");


    //    // Create the time label

    //    m_timeLabel = new QLabel( messageLayer);
    //    m_timeLabel->setText(time);
    //    m_timeLabel->setFixedWidth(100);
    //    m_timeLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignBottom);
    //    m_timeLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 2px;");

    //    //create the layout of inside of messsage

    //    QVBoxLayout* layout_inside = new QVBoxLayout(messageLayer);

    //    setfileField(text);
    //    layout_inside->addWidget(FileDownload_Open_field);
    //    layout_inside->addWidget(m_textLabel);
    //    layout_inside->addWidget(m_timeLabel);


    //    setAlignmentOfmessage(layout_inside,IsSentMessage);
    //    messageLayer->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);

    //    // Create the layout of all of message
    //    QVBoxLayout* layout = new QVBoxLayout(this);

    //    layout->addWidget(messageLayer);

    //    setAlignmentOfmessage(layout,IsSentMessage);
    //    //layout->setAlignment(Qt::AlignLeft);


    //    this->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Fixed);


    //    // Set the size and style

    //    messageLayer->setStyleSheet("background-color: "+background_color.name()+"; border: 1px solid #cccccc; border-radius: 20px; padding: 10px;");


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
        fileIcon= QPixmap("DataFiles\\pngwing.png");
    else
    {
        fileIcon= QPixmap("DataFiles\\downloadedFIleIcon.png");
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

void FileMessageWidget::btn_file_clicked()
{
    QString filename = lbl_title->text();
    if(!checkDownloaded(lbl_title->text()))
    {
        emit downloadFile();
        setIconOfCircle(false);
    }
    else
    {
        QString filePath = "files/" + filename;
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));


    }

}
