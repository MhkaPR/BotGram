#include "chatpage.h"
#include "ui_chatpage.h"
#include<QAbstractSlider>


#include <QtDebug>
chatPage::chatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatPage)
{
    ui->setupUi(this);

    ui->scrollArea->setWidgetResizable(true);

    ScrollWidget = new QWidget;

    ScrollLayout = new QVBoxLayout;


    //    QString textField ="Lorem ipsum dolor sit\n amet, consectetur adipiscing elit. Sed \nut perspiciatis unde omnis iste na\ntus error sit voluptatem accusantium doloremque laudant\n\nium.";
    //    // textField = messageWidget::wordWrap(textField,20);

    //    // qDebug() << textField;
    //    messageWidget* newmessageWid = new messageWidget(textField,"10:15",this);

    //    messageWidget* newmessageWid2 = new messageWidget("Are you Good","22:05",this);
    //    messageWidget* newmessageWid3 = new messageWidget("goodbye","01:01",this);
    //    messageWidget* newmessageWid4 = new messageWidget("goodbye","01:01",this);

    //    ScrollLayout->addWidget(newmessageWid);
    //    ScrollLayout->addWidget(newmessageWid2);
    //    ScrollLayout->addWidget(newmessageWid3);
    //    ScrollLayout->addWidget(newmessageWid4);

    ScrollLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    ScrollWidget->setLayout(ScrollLayout);
    //ScrollWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    ui->scrollArea->setWidget(ScrollWidget);



}

chatPage::~chatPage()
{
    delete ui;
}

void chatPage::addMessage(messageWidget *newMsg)
{
    ScrollLayout->addWidget(newMsg);
    //ScrollWidget->setLayout(ScrollLayout);
    ScrollWidget->setLayout(ScrollLayout);
   //ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    // Get a pointer to the last widget in the layout
    QWidget* lastWidget = ScrollLayout->itemAt(ScrollLayout->count() - 1)->widget();

    // Scroll to the last widget
    ui->scrollArea->ensureWidgetVisible(lastWidget);


}
