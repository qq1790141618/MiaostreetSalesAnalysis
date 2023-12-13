#ifndef GOODSLIST_H
#define GOODSLIST_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class goodslist;
}

class goodslist : public QWidget
{
    Q_OBJECT

public:
    explicit goodslist(QWidget *parent = nullptr);
    ~goodslist();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::goodslist *ui;
    QUrl pageResource = QUrl("qrc:/pages/goodslist/index.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // GOODSLIST_H
