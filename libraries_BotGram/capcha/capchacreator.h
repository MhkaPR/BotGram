#ifndef CAPCHACREATOR_H
#define CAPCHACREATOR_H

#include <QObject>
#include <QLabel>
#include <string>
class capchaCreator
{
public:
    capchaCreator(const int X,const int Y,QLabel* T1,QLabel* T2,QLabel* T3,QLabel* T4,QLabel* Backg,QLabel* Noise);
    void build_capcha();



private:
    enum colors
    {
        BLACK,
        RED,
        BLUE,
        GREEN,
        YELLOW,
    };
    int xCapcha,yCapcha;
    QLabel *t1,*t2,*t3,*t4,*noise,*background;

    short int ct1,ct2,ct3,ct4;
    short int pt1[2],pt2[2],pt3[2],pt4[2];
    QString noiseFile;


      void build_capcha_char(QLabel *t);
    int build_fontSize();
    QString build_color();
    char build_char();
    short int* build_Padding();
    QString choose_noise();
    bool build_booleansFont();




};

#endif // CAPCHACREATOR_H
