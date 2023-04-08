#include "botgrameenv.h"
#include "ui_botgrameenv.h"

BotGrameENV::BotGrameENV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BotGrameENV)
{
    ui->setupUi(this);
}

BotGrameENV::~BotGrameENV()
{
    delete ui;
}
