#ifndef BOTGRAMEENV_H
#define BOTGRAMEENV_H

#include <QWidget>

namespace Ui {
class BotGrameENV;
}

class BotGrameENV : public QWidget
{
    Q_OBJECT

public:
    explicit BotGrameENV(QWidget *parent = nullptr);
    ~BotGrameENV();

private:
    Ui::BotGrameENV *ui;
};

#endif // BOTGRAMEENV_H
