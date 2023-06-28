#include "messagewidget.h"
#include<QListWidget>

messageWidget::messageWidget(const QString& text,QString time, QWidget* parent,bool IsSentMessage)
    : QWidget(parent)
{




    this->setMinimumHeight(100);


    setBacground_color(IsSentMessage);
    messageLayer = new QWidget(this);

    messageLayer->setMaximumWidth(parent->geometry().width()/5*4);
    messageLayer->setMinimumHeight(100);


    // Create the text label
    m_textLabel = new QLabel(messageLayer);


    //m_textLabel->setMaximumWidth(parent->maximumWidth());
    QFontMetrics fm(m_textLabel->font());
    int Ach =fm.horizontalAdvance("W");



    //word wrapping

    int lenText = text.length();

    QString newText=text;
    if(newText.isEmpty()) newText =" ";
    if(lenText > 50) newText = wordWrap(text,50);
    m_textLabel->setWordWrap(true);


    //fix message
    m_textLabel->setText(newText);

    m_textLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Maximum);
    m_textLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignTop);

    m_textLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 10px;");


    // Create the time label

    m_timeLabel = new QLabel( messageLayer);
    m_timeLabel->setText(time);
    m_timeLabel->setFixedWidth(100);
    m_timeLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignBottom);
    m_timeLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 2px;");

    //create the layout of inside of messsage


    layout_inside = new QVBoxLayout(messageLayer);

    layout_inside->addWidget(m_textLabel);
    layout_inside->addWidget(m_timeLabel);

    setAlignmentOfmessage(layout_inside,IsSentMessage);
    messageLayer->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);



    // Create the layout of all of message
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(messageLayer);


    setAlignmentOfmessage(layout,IsSentMessage);
    //layout->setAlignment(Qt::AlignLeft);


    this->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    // Set the size and style

    messageLayer->setStyleSheet("background-color: "+background_color.name()+"; border: 1px solid #cccccc; border-radius: 20px; padding: 10px;");

}

QString messageWidget::wordWrap(QString inputText,int maxWidth)
{

    //    //oooo oooooooooo ooooooooo ooooo

    //    QString wrappedText;
    //    QString currentLine;
    //    QString current_Word;
    //    int WordAndis = 0;
    //    int len_Word =0;
    //    int lineLen =0;
    //    int lenInputText = inputText.length();
    //    for (int i = 0; i < lenInputText; i++) {


    //        //oo oooo oo o  oooooo ppooooooooooooooooooooooooo ooo
    //        //5
    //        //o
    //        //1
    //        QChar chTemp = inputText.at(i);
    //        if(lineLen >= maxWidth)
    //        {
    //            if(chTemp == ' ')
    //            {
    //                wrappedText +='\n';
    //                lineLen =0;
    //            }
    //            else if(chTemp == '\n')
    //            {
    //                lineLen = 0;
    //            }
    //            else
    //            {
    //                wrappedText.remove(WordAndis,len_Word);
    //            }
    //        }
    //        else
    //        {

    //        }


    //        wrappedText += chTemp;
    //        lineLen++;

    //        if(chTemp != ' ')
    //        {
    //            if(inputText[i-1] == ' ')
    //                WordAndis = i;

    //            len_Word++;
    //        }
    //        else {
    //            len_Word=0;
    //        }






    //        //        if(chTemp == ' ' || (lenInputText == i+1))
    //        //        {
    //        //            int len_currentline = currentLine.size();
    //        //            int len_currentWord = current_Word.size();
    //        //            if(maxWidth < (len_currentline+len_currentWord-1))
    //        //            {
    //        //                wrappedText +=currentLine+"\n";
    //        //                currentLine.clear();
    //        //                currentLine+=current_Word;
    //        //                current_Word.clear();
    //        //            }
    //        //            else if(maxWidth == (len_currentline+len_currentWord-1))
    //        //            {
    //        //                current_Word.remove(len_currentWord-1);
    //        //                currentLine +=current_Word;
    //        //                wrappedText +=currentLine+"\n";
    //        //                currentLine.clear();
    //        //                current_Word.clear();
    //        //                currentLine.append(' ');
    //        //            }
    //        //            else
    //        //            {
    //        //                currentLine +=current_Word;
    //        //                current_Word.clear();
    //        //            }
    //        //            if((current_Word.size() >= maxWidth))
    //        //            {
    //        //                int ans = current_Word.size()/maxWidth;
    //        //                for (int i=1;i <= ans; i++)
    //        //                {
    //        //                    current_Word.insert(i*maxWidth-1,'\n');


    //        //                }
    //        //            }


    //    }


    //    return wrappedText;


    //QString inputText = "oooo oooooooooo ooooooooo ooooo";
    //int maxWidth = 7;
    QString wrappedText;
    QString currentLine;
    //    QString current_Word;
    int WordAndis = 0;
    int len_Word =0;
    int lineLen =0;
    int lenInputText = inputText.size();
    bool isAnySpace = false;
    for (int i = 0; i < lenInputText; i++) {
        // qDebug() << "hello";

        //oo oooo oo o  oooooo ppooooooooooooooooooooooooo ooo
        //5
        //o
        //1
        QChar chTemp = inputText.at(i);
        if(lineLen >= maxWidth)
        {
            if(chTemp == ' ')
            {
                wrappedText +='\n';
                isAnySpace =false;
                lineLen =0;
            }
            else if(chTemp == '\n')
            {
                isAnySpace = false;
                lineLen = 0;
            }
            else
            {
                if(isAnySpace)
                {
                    // QString tempWord =  wrappedText.right(len_Word);
                    //                wrappedText.chop(len_Word);
                    //                wrappedText += '\n';
                    //                wrappedText += tempWord;
                    wrappedText.insert(WordAndis,'\n');
                    isAnySpace =false;

                    lineLen = len_Word;

                }
                else
                {
                    wrappedText +="\n";
                    lineLen =0;
                    len_Word =0;
                }
            }
        }
        else
        {

        }


        wrappedText += chTemp;
        lineLen++;

        if(chTemp != ' ')
        {
            if(i!=0)
                if(inputText[i-1] == ' ' ||
                        inputText[i-1] == '\n')
                    WordAndis = i;

            len_Word++;
        }
        else {
            isAnySpace =true;
            len_Word=0;
        }

    }

    //qDebug() << inputText << "\n"<< wrappedText;

    return wrappedText;

}

void messageWidget::setBacground_color(bool value)
{
    if(value)
    {
        background_color.setRgb(180,255,192);
    }
    else {
        background_color.setRgb(255,255,255);
    }
}

void messageWidget::setBacground_color(QColor color)
{
    background_color = color;
}

void messageWidget::setAlignmentOfmessage(QVBoxLayout *layout,bool value)
{
    Qt::AlignmentFlag flag;
    if(value) flag = Qt::AlignLeft;
    else flag = Qt::AlignRight;
    layout->setAlignment(flag);
}

void messageWidget::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::RightButton) {
        QMenu menu(this);
        QAction *CopyAction = menu.addAction("Copy");
        QAction *DeleteAction = menu.addAction("Delete message (Not complate)");
        QAction *pinAction = menu.addAction("pin");
        QAction *selectedAction = menu.exec(event->globalPos());
        if (selectedAction == CopyAction) {
            QClipboard *Copy = QApplication::clipboard();
            Copy->setText(m_textLabel->text());
            // Handle Action 1
        } else if (selectedAction == DeleteAction) {
            // Handle Action 2
        }
        else if (selectedAction == pinAction) {

            emit pinnedStatusChanged(true);



    } else {

        QWidget::mousePressEvent(event);
    }
    //QWidget::mousePressEvent(event);
         } }
void messageWidget::setPinned(bool pinned)
{
    //m_isPinned = pinned;
    //m_pinLabel->setVisible(pinned);
    //emit pinnedStatusChanged(pinned);
}
