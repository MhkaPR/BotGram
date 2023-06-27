#include "userboxwidget.h"

UserBoxWidget::UserBoxWidget(QPixmap ImageOfProfile,QString Name,QString TweLineOfLastMessage,QString time, QWidget* parent)
    : QWidget(parent)
{
    lbl_name.setText(Name);
    lbl_name.setStyleSheet("border: 0px solid #cccccc;"
                           "font-size:20px;color:#000000 ;"
                           "padding: 3px;"
                           );
    lbl_TweLineOfLastMessages.setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Fixed);

    const int WordWrapLen = 50;
    QStringList TweLines;
    if(TweLineOfLastMessage.length() > WordWrapLen)
    {
        TweLines= wordWrap(TweLineOfLastMessage,WordWrapLen).split("\n");
        TweLineOfLastMessage = (TweLines[0]+"\n"+TweLines[1]);
    }

    lbl_TweLineOfLastMessages.setText(TweLineOfLastMessage);
    lbl_TweLineOfLastMessages.setStyleSheet("border: 0px solid #cccccc;"
                                            "color:#666666 ;"
                                            "padding: 3px;"
                                            );
    lbl_TweLineOfLastMessages.setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Fixed);

    lbl_time.setText(time);
    lbl_time.setStyleSheet("border: 0px solid #cccccc;"
                           "color:#666666 ;"
                           "padding: 3px;"
                           );
    lbl_time.setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    lbl_image.setPixmap(ImageOfProfile);
    lbl_image.setFixedSize(100,100);
    lbl_image.setStyleSheet("border: 1px solid #cccccc;"
                            " border-radius: 50px;"
                            " padding: 1px;"
                            );
    lbl_image.setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);



    // add in right layout
    RightLayoutForObjects.addWidget(&lbl_name);
    RightLayoutForObjects.addWidget(&lbl_TweLineOfLastMessages);
    RightLayoutForObjects.addWidget(&lbl_time);

    complateRightObjects.setLayout(&RightLayoutForObjects);
    complateRightObjects.setStyleSheet("border: 0px solid #cccccc;");


    //add in main layout
    mainLayout.addWidget(&lbl_image);
    mainLayout.addWidget(&complateRightObjects);


    this->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    this->setStyleSheet("border: 1px solid #cccccc;");

    this->setLayout(&mainLayout);

















    //    // Create the text label
    //    m_textLabel = new QLabel(messageLayer);


    //    //m_textLabel->setMaximumWidth(parent->maximumWidth());
    //    QFontMetrics fm(m_textLabel->font());
    //    int Ach =fm.horizontalAdvance("W");



    //    //word wrapping

    //    int lenText = text.length();

    //    QString newText=text;
    //    if(newText.isEmpty()) newText =" ";
    //    if(lenText > 50) newText = wordWrap(text,50);
    //    m_textLabel->setWordWrap(true);


    //    //fix message
    //    m_textLabel->setText(newText);

    //    m_textLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Maximum);
    //    m_textLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignTop);

    //    m_textLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 10px;");


    //    // Create the time label

    //    m_timeLabel = new QLabel( messageLayer);
    //    m_timeLabel->setText(time);
    //    m_timeLabel->setFixedWidth(100);
    //    m_timeLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignBottom);
    //    m_timeLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 2px;");

    //    //create the layout of inside of messsage


    //    layout_inside = new QVBoxLayout(messageLayer);

    //    layout_inside->addWidget(m_textLabel);
    //    layout_inside->addWidget(m_timeLabel);

    //    setAlignmentOfmessage(layout_inside,IsSentMessage);
    //    messageLayer->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);



    //    // Create the layout of all of message
    //    QVBoxLayout* layout = new QVBoxLayout(this);

    //    layout->addWidget(messageLayer);


    //    setAlignmentOfmessage(layout,IsSentMessage);
    //    //layout->setAlignment(Qt::AlignLeft);


    //    this->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    //    // Set the size and style

    //    messageLayer->setStyleSheet("background-color: "+background_color.name()+"; border: 1px solid #cccccc; border-radius: 20px; padding: 10px;");

}

QString UserBoxWidget::wordWrap(QString inputText, int maxWidth)
{
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
