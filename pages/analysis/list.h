#ifndef SALESANDREFUNDSLISTPAGE_H
#define SALESANDREFUNDSLISTPAGE_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class SalesAndRefundsListPage;
}

class SalesAndRefundsListPage : public QWidget
{
    Q_OBJECT

public:
    explicit SalesAndRefundsListPage(QWidget *parent = nullptr);
    ~SalesAndRefundsListPage();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::SalesAndRefundsListPage *ui;
    QUrl pageResource = QUrl("qrc:/pages/analysis/list.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // SALESANDREFUNDSLISTPAGE_H
