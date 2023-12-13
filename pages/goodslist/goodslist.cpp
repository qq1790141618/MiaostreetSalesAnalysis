#include "goodslist.h"
#include "ui_goodslist.h"
#include "bridge.h"

#include <QFile>
#include <QSettings>

goodslist::goodslist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::goodslist)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);

    QMovie* movie = new QMovie(":/images/loading-2.gif");
    movie->setScaledSize(QSize(20, 20));
    ui->label_gif->setMovie(movie);
    ui->label_gif->movie()->start();

    mainViewer = new QWebEngineView();
    mainViewer->setContextMenuPolicy(Qt::NoContextMenu);
    mainViewer->page()->setBackgroundColor(QColor("#f2f3ff"));

    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
    settings->beginGroup("PAGESIZE");
    int pagesize = settings->value("GOODS_LIST_TABLE", 10).toInt();
    settings->endGroup();

    connect(mainViewer, &QWebEngineView::loadFinished, this, [=] {
        QString javascriptCode = QString("createPage('%1', '%2', '%3', '%4')").arg(serve).arg(store).arg(brand).arg(pagesize);
        mainViewer->page()->runJavaScript(javascriptCode);
    });

    ui->verticalLayout->addWidget(mainViewer);

    QWebChannel *webChannel = new QWebChannel();
    Bridge *webobj = new Bridge();
    webChannel->registerObject("webobj", webobj);
    mainViewer->page()->setWebChannel(webChannel);
    connect(webobj, &Bridge::showIt, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
    });
}

goodslist::~goodslist()
{
    delete ui;
}
