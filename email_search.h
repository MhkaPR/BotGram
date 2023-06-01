#ifndef EMAIL_SEARCH_H
#define EMAIL_SEARCH_H

#include <QWidget>
#include<QtNetwork>

namespace Ui {
class email_search;
}

class email_search : public QWidget
{
    Q_OBJECT

public:

    explicit email_search(QTcpSocket *so,QWidget *parent = nullptr);
    ~email_search();

private slots:
    void on_pushButton_clicked();


private:
    Ui::email_search *ui;
    QTcpSocket *socket = nullptr;
};

#endif // EMAIL_SEARCH_H
