#include "messagewidget.h"

messageWidget::messageWidget(const QString& text,QString time, QWidget* parent,bool IsSentMessage)
    : QWidget(parent)
{




    this->setMinimumHeight(100);




    setBacground_color(IsSentMessage);
    QWidget *messageLayer = new QWidget(this);

    messageLayer->setMaximumWidth(parent->geometry().width()/5*4);
    messageLayer->setMinimumHeight(100);


    // Create the text label
    m_textLabel = new QLabel(messageLayer);


    m_textLabel->setMaximumWidth(this->maximumWidth());
    QFontMetrics fm(m_textLabel->font());
    int Ach =fm.horizontalAdvance("W");



    //word wrapping
    int lenText = text.length();
    QString newText=text;
    if(lenText >= 100) newText = wordWrap(text,(parent->geometry().width()/5*4)/Ach);
    m_textLabel->setWordWrap(true);

    //fix message
    m_textLabel->setText(newText);
    m_textLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Fixed);
    m_textLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignTop);

    m_textLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 10px;");


    // Create the time label

    m_timeLabel = new QLabel( messageLayer);
    m_timeLabel->setText(time);
    m_timeLabel->setFixedWidth(100);
    m_timeLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignBottom);
    m_timeLabel->setStyleSheet("background-color: "+background_color.name()+"; border: 0px solid #cccccc;padding: 2px;");

    //create the layout of inside of messsage

    QVBoxLayout* layout_inside = new QVBoxLayout(messageLayer);

    layout_inside->addWidget(m_textLabel);
    layout_inside->addWidget(m_timeLabel);

    setAlignmentOfmessage(layout_inside,IsSentMessage);
    messageLayer->setSizePolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);

    // Create the layout of all of message
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(messageLayer);

    setAlignmentOfmessage(layout,IsSentMessage);
    //layout->setAlignment(Qt::AlignLeft);


    this->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Fixed);


    // Set the size and style

    messageLayer->setStyleSheet("background-color: "+background_color.name()+"; border: 1px solid #cccccc; border-radius: 20px; padding: 10px;");

}

QString messageWidget::wordWrap(QString inputText,int maxWidth)
{

    //    QString currentLine = "";
    //    QString outputText = "";
    //    QString word;
    //    int i = 0;

    //    while (i < inputText.length()) {
    //        // Read the next word
    //        word = "";
    //        while (i < inputText.length() && inputText[i] != ' ') {
    //            word += inputText[i];
    //            i++;
    //        }

    //        // Add the word to the current line
    //        if (currentLine.length() + word.length() + 1 <= maxWidth) {
    //            if (currentLine.length() > 0) {
    //                currentLine += " ";
    //            }
    //            currentLine += word;
    //        }
    //        // Start a new line with the current word
    //        else {
    //            outputText += currentLine + '\n';
    //            currentLine = word;
    //        }
    //        i++;
    //    }

    //    // Add the last line to the output text
    //    outputText += currentLine;

    //    return outputText;
    QString wrappedText;
    QString currentLine;

    for (int i = 0; i < inputText.length(); i++) {
        QChar ch = inputText.at(i);
        if (ch == ' ') {
            if (currentLine.size() + 1 > maxWidth) {
                wrappedText += currentLine + "\n";
                currentLine.clear();
            } else {
                currentLine += ch;
            }
        } else {
            if (currentLine.size() >= maxWidth) {
                wrappedText += currentLine + "\n";
                currentLine.clear();
            }
            currentLine += ch;
        }
    }

    if (!currentLine.isEmpty()) {
        wrappedText += currentLine;
    }

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
