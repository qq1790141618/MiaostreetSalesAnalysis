#ifndef SALESANALYSIS_H
#define SALESANALYSIS_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class SalesAnalysis;
}

class SalesAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SalesAnalysis(QWidget *parent = nullptr);
    ~SalesAnalysis();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::SalesAnalysis *ui;
    QUrl pageResource = QUrl("qrc:/pages/analysis/sales.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // SALESANALYSIS_H
