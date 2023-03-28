#ifndef BOTGRAM_H
#define BOTGRAM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class botgram; }
QT_END_NAMESPACE

class botgram : public QMainWindow
{
    Q_OBJECT

public:
    botgram(QWidget *parent = nullptr);
    ~botgram();

private slots:
    void on_StartApplication_btn_clicked();

private:
    Ui::botgram *ui;
};
#endif // BOTGRAM_H
