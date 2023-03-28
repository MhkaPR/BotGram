#include "botgram.h"
#include "ui_botgram.h"

botgram::botgram(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::botgram)
{
    ui->setupUi(this);
}

botgram::~botgram()
{
    delete ui;
}


void botgram::on_StartApplication_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
