#include "buildercapcha.h"
#include "ui_buildercapcha.h"


BuilderCapcha::BuilderCapcha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuilderCapcha)
{
    ui->setupUi(this);

    cp.randomize();
    cp.setDifficulty(2);
    cp.generateText(7);
    if(!sw) ggg=cp.captchaImage();
    // painter.drawImage(0,0,ggg);
    if(!ggg.isNull())
        ui->lbl_capcha->setPixmap(QPixmap::fromImage(ggg));

    if(!sw)
    {
        strCaptcha=cp.captchaText();
    }
    sw=true;



}

BuilderCapcha::~BuilderCapcha()
{
    delete ui;
}

void BuilderCapcha::buildAgain()
{
    sw=false;
    cp.randomize();
    cp.setDifficulty(2);
    cp.generateText(7);
    if(!sw) ggg=cp.captchaImage();
    // painter.drawImage(0,0,ggg);
    if(!ggg.isNull())
        ui->lbl_capcha->setPixmap(QPixmap::fromImage(ggg));

    if(!sw)
    {
        strCaptcha=cp.captchaText();
    }
    sw=true;
}

void BuilderCapcha::on_btn_again_clicked()
{
    sw=false;
    cp.randomize();
    cp.setDifficulty(2);
    cp.generateText(7);
    if(!sw) ggg=cp.captchaImage();
    // painter.drawImage(0,0,ggg);
    if(!ggg.isNull())
        ui->lbl_capcha->setPixmap(QPixmap::fromImage(ggg));

    if(!sw)
    {
        strCaptcha=cp.captchaText();
    }
    sw=true;
}

void BuilderCapcha::on_btn_again_pressed()
{
    ui->btn_again->setStyleSheet("color: white;"
                                  "border-radius:25px;"
                                  "background-color:#2097b5");
}

void BuilderCapcha::on_btn_again_released()
{
    ui->btn_again->setStyleSheet(
                " color: rgb(131, 155, 166);"
                "  border-radius:25px;"
                "  border:1px solid;"
                " border-color: rgb(131, 155, 151);");
}
