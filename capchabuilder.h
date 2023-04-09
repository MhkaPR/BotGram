#ifndef CAPCHABUILDER_H
#define CAPCHABUILDER_H

#include <QWidget>
#include "DataFiles/checkingcaptcha/captcha.h"
#include "botgram.h"
namespace Ui {
class CapchaBuilder;
}

class CapchaBuilder : public QWidget
{
    Q_OBJECT

public:

     Ui::CapchaBuilder *ui;
    explicit CapchaBuilder(QWidget *parent = nullptr);
    ~CapchaBuilder();
    void paintEvent(QPaintEvent *);
    //friend BotGram;
QString strCaptcha;

private:

    bool sw=false;
    QImage ggg;
    Captcha cp;
};

#endif // CAPCHABUILDER_H
