#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QtWebEngineWidgets>

namespace Ui {
class PageView;
}

class PageView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit PageView(QWidget *parent = nullptr);
    ~PageView();
    void setLoadedJs(const QString js);

signals:
    void open(QString page);

private:

};

#endif // PAGEVIEW_H
