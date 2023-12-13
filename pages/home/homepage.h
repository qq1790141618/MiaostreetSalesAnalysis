#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

signals:
    void open(const QString &page);

private:
    Ui::HomePage *ui;
    QUrl pageResource = QUrl("qrc:/pages/home/index.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // HOMEPAGE_H
