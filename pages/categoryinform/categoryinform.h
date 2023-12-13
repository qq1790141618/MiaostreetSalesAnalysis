#ifndef CATEGORYINFORM_H
#define CATEGORYINFORM_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class categoryinform;
}

class categoryinform : public QWidget
{
    Q_OBJECT

public:
    explicit categoryinform(QWidget *parent = nullptr);
    ~categoryinform();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::categoryinform *ui;
    QUrl pageResource = QUrl("qrc:/pages/categoryinform/index.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // CATEGORYINFORM_H
