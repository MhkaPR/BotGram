#include "capchabuilder.h"
#include "ui_capchabuilder.h"
#include "botgram.h"
CapchaBuilder::CapchaBuilder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CapchaBuilder)
{
    ui->setupUi(this);
}

CapchaBuilder::~CapchaBuilder()
{
    delete ui;
}
//start of capcha creator
void CapchaBuilder::paintEvent(QPaintEvent *)
{


    QPainter painter(this);
//   Captcha cp;



   cp.setDifficulty(3);
   cp.generateText(7,true,false,false);
   if(!sw) ggg=cp.captchaImage();
   painter.drawImage(0,0,ggg);

    if(!sw)
    {
        strCaptcha=cp.captchaText();
    }
    sw=true;
    /*cp.randomize();

    cp.setDifficulty(0);
    //cp.loadDictionary("dictionary.txt");
    //cp.setTextGeneration(Captcha::TextGeneration_Dictionary);

    cp.generateText();
    //ui->label->setText(cp.captchaText());


    painter.drawImage(0, 0, cp.captchaImage());*/


}
//end of capcha creator-----------------------
