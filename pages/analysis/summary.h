#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class SummaryPage;
}

class SummaryPage : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryPage(QWidget *parent = nullptr);
    ~SummaryPage();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::SummaryPage *ui;
    QUrl pageResource = QUrl("qrc:/pages/analysis/summary.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // SUMMARYPAGE_H
