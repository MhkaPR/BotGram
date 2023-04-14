#ifndef BUILDERCAPCHA_H
#define BUILDERCAPCHA_H

#include <QWidget>
#include "DataFiles/checkingcaptcha/captcha.h"

namespace Ui {
class BuilderCapcha;
}

class BuilderCapcha : public QWidget
{
    Q_OBJECT

public slots:
    void on_btn_again_clicked();
public:
    QString strCaptcha;
    explicit BuilderCapcha(QWidget *parent = nullptr);
    ~BuilderCapcha();
    void buildAgain();

private slots:
   // void on_btn_again_clicked();

    void on_btn_again_pressed();

    void on_btn_again_released();

private:
    Ui::BuilderCapcha *ui;
    bool sw=false;
    QImage ggg;
    Captcha cp;
};

#endif // BUILDERCAPCHA_H
