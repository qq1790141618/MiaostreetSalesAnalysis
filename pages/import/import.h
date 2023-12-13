#ifndef IMPORTPAGE_H
#define IMPORTPAGE_H

#include <QWidget>
#include <QtWebEngineWidgets>

namespace Ui {
class ImportPage;
}

class ImportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ImportPage(QWidget *parent = nullptr);
    ~ImportPage();
    void setParam(QString s, QString st, QString br){
        serve = s;
        store = st;
        brand = br;
        mainViewer->load(QUrl(pageResource));
    }

private:
    Ui::ImportPage *ui;
    QUrl pageResource = QUrl("qrc:/pages/import/index.html");
    QWebEngineView *mainViewer;
    QString serve;
    QString store;
    QString brand;
};

#endif // IMPORTPAGE_H
