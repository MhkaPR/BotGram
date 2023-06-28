#include "userboxwidget.h"

UserBoxWidget::UserBoxWidget(QPixmap ImageOfProfile,QString Name,QString TweLineOfLastMessage,QString time,int unreadMessageCount, QWidget* parent)
    : QWidget(parent)
{

    //name
    lbl_name.setText(Name);
    lbl_name.setStyleSheet("border: 0px solid #cccccc;"
                           "font-size:20px;color:#000000 ;"
                           "padding: 3px;"
                           );
    lbl_name.setAlignment(Qt::AlignmentFlag::AlignLeft);
    lbl_name.setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);


    //tweline
    const int WordWrapLen = 50;
    QStringList TweLines;
    if(TweLineOfLastMessage.length() > WordWrapLen)
    {
        TweLines= wordWrap(TweLineOfLastMessage,WordWrapLen).split("\n");
        TweLineOfLastMessage = (TweLines[0]+"\n"+TweLines[1]);
        if(TweLines[1].length() == WordWrapLen)
            TweLineOfLastMessage += " ...";
    }
    else
    {
        TweLineOfLastMessage +="\n ";
    }



    lbl_TweLineOfLastMessages.setText(TweLineOfLastMessage);
    lbl_TweLineOfLastMessages.setStyleSheet("border: 0px solid #cccccc;"
                                            "color:#666666 ;"
                                            "padding: 3px;"

                                            );
    lbl_TweLineOfLastMessages.setAlignment(Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignLeft);
    lbl_TweLineOfLastMessages.setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);


    //time
    lbl_time.setText(time);
    lbl_time.setStyleSheet("border: 0px solid #cccccc;"
                           "color:#666666 ;"
                           "padding: 3px;"
                           );
    lbl_time.setAlignment(Qt::AlignmentFlag::AlignRight);
    lbl_time.setMinimumWidth(30);
    lbl_time.setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Expanding);

    //lbl_time.setStyleSheet("background-color:black;");


    //imgae
    lbl_image.setPixmap(ImageOfProfile);
    lbl_image.setFixedSize(100,100);
    lbl_image.setStyleSheet("border: 1px solid #cccccc;"
                            " border-radius: 50px;"
                            " padding: 1px;"
                            );
    lbl_image.setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    //unread mesages
    lbl_UnreadMessagesCount.setFixedSize(QSize(30,30));
    lbl_UnreadMessagesCount.setText(QString::number(unreadMessageCount));
    lbl_UnreadMessagesCount.setAlignment(Qt::AlignmentFlag::AlignCenter);
    lbl_UnreadMessagesCount.setStyleSheet("background-color:#00A2E8;border-radius:15px;color:#ffffff");
    if(!unreadMessageCount)
    {
        lbl_UnreadMessagesCount.setVisible(false);
    }

    lbl_UnreadMessagesCount.setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);

    //fix name and time
    nameANDtimeLayout.setParent(this);
    nameANDtimeWidget.setParent(this);

    nameANDtimeLayout.addWidget(&lbl_name);
    nameANDtimeLayout.addWidget(&lbl_time);

    nameANDtimeWidget.setLayout(&nameANDtimeLayout);


    //fix tweline and unread messages
    TweLineANDunreadMessagesLayout.addWidget(&lbl_TweLineOfLastMessages);
    TweLineANDunreadMessagesLayout.addWidget(&lbl_UnreadMessagesCount);

    TweLineANDunreadMessagesWidget.setLayout(&TweLineANDunreadMessagesLayout);


    // add in right layout

    //RightLayoutForObjects.addWidget(&lbl_name);

    RightLayoutForObjects.addWidget(&nameANDtimeWidget);
    RightLayoutForObjects.addWidget(&TweLineANDunreadMessagesWidget);
    //RightLayoutForObjects.addWidget(&lbl_time);

    complateRightObjects.setLayout(&RightLayoutForObjects);
    complateRightObjects.setStyleSheet("border: 0px solid #cccccc;");


    //add in main layout
    mainLayout.addWidget(&lbl_image);
    mainLayout.addWidget(&complateRightObjects);


    this->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);


    this->setStyleSheet("border: 1px solid #cccccc;");

    this->setLayout(&mainLayout);


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

QString UserBoxWidget::getTweLine(QString text, int len)
{

    if(text.length() > len)
    {
        QStringList textList = wordWrap(text,len).split("\n");
        text = textList[0]+"\n"+textList[1];

    }
    return  text;

}

void UserBoxWidget::addUnReadmessageCount(int count)
{

    int count_Now = lbl_UnreadMessagesCount.text().toInt();
    lbl_UnreadMessagesCount.setVisible(true);
    lbl_UnreadMessagesCount.setText(QString::number(count+count_Now));
}

