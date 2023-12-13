#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class dashboard;
}

class dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit dashboard(QWidget *parent = nullptr);
    ~dashboard();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::dashboard *ui;
    QUrl pageResource = QUrl("qrc:/pages/dashboard/index.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // DASHBOARD_H
