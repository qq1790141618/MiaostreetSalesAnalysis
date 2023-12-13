#include "pageview.h"
#include "bridge.h"

PageView::PageView(QWidget *parent) :
    QWebEngineView(parent)
{
    setContextMenuPolicy(Qt::NoContextMenu);
    page()->setBackgroundColor(QColor("#f2f3ff"));

    QWebChannel *webChannel = new QWebChannel();
    Bridge *webobj = new Bridge();
    webChannel->registerObject("webobj", webobj);
    page()->setWebChannel(webChannel);
    connect(webobj, &Bridge::showIt, this, [=]{
        show();
    });
    connect(webobj, &Bridge::open, this, [=](QString page){
        emit open(page);
    });
}

PageView::~PageView()
{

}

void PageView::setLoadedJs(QString js){
    connect(this, &QWebEngineView::loadFinished, this, [=] {
        page()->runJavaScript(js);
    });
}
