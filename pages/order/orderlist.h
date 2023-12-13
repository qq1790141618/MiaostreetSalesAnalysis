#ifndef ORDERLIST_H
#define ORDERLIST_H

#include <QWidget>

namespace Ui {
class OrderList;
}

class OrderList : public QWidget
{
    Q_OBJECT

public:
    explicit OrderList(QWidget *parent = nullptr);
    ~OrderList();
    void setParam(QString st, QString br){
        store = st;
        brand = br;
    }


signals:
    void openPage(const QString &page);

private:
    Ui::OrderList *ui;
    QString store;
    QString brand;
};

#endif // ORDERLIST_H
