#include "capchacreator.h"
#include <botgram.h>

capchaCreator::capchaCreator(const int X,const int Y,QLabel* T1,QLabel* T2,QLabel* T3,QLabel* T4,QLabel* Backg,QLabel* Noise)

{
    xCapcha = X;
    yCapcha = Y;
    background=Backg;
    background->setGeometry(xCapcha,yCapcha,Backg->geometry().width(),Backg->geometry().height());
    background->setText("");
    background->setStyleSheet("background-color:rgb(255,255,255);");

}

void capchaCreator::build_capcha()
{
    build_capcha_char(t1);
    build_capcha_char(t2);
    build_capcha_char(t3);
    build_capcha_char(t4);
}

void capchaCreator::build_capcha_char(QLabel *t)
{
    //t1
    t->setText(QString(build_char()));
    QFont f;
    f.setPointSize(build_fontSize());

    f.setBold(build_booleansFont());
    f.setItalic(build_booleansFont());
    f.setStrikeOut(build_booleansFont());

    f.setFamily("Nazanin");
    t->setFont(f);

    QString color = "color:"+build_color();
    short *p=NULL;
       p = build_Padding();
    QString paddingL ="padding-left:" + QString(p[0])+"px;";
     QString paddingT ="padding-top:" + QString(p[1])+"px;";
     QString colorMe="color:"+build_color()+";";




}

int capchaCreator::build_fontSize()
{
    srand(time(0));
    int temp = rand()%25 +15;
    return temp;
}

QString capchaCreator::build_color()
{
    srand(time(0));
    int temp = rand()%5;
    switch (temp) {
    case BLACK:return "black";
    case RED:return "red";
    case BLUE:return "blue";
    case YELLOW:return "yellow";
    case GREEN:return "green";
    }
}

char capchaCreator::build_char()
{
    string check = "qwertyuiopBNM123asdfghjmQWERTYUk56789lzxcvbnIOPASDFGHJKLZXCV40";
    srand(time(0));
    int temp = rand()%62;
    return check[temp];
}

short *capchaCreator::build_Padding()
{
    short *p=new short[2];
    srand(time(0));
    p[0] = rand()%7;
    p[1] = rand()%7;
    return p;

}

QString capchaCreator::choose_noise()
{
    srand(time(0));
    short numTemp= rand()%5+1;
    QString temp="background-image: url(:/Eye/noise"+ QString(numTemp)+ ".png);";
    return temp;

}

bool capchaCreator::build_booleansFont()
{
    srand(time(0));
    return rand()%2;
}
