﻿#include "list.h"
#include "ui_list.h"
#include "bridge.h"

#include <QFile>

SalesAndRefundsListPage::SalesAndRefundsListPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesAndRefundsListPage)
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
    connect(mainViewer, &QWebEngineView::loadFinished, this, [=] {
        QString javascriptCode = QString("createPage('%1', '%2', '%3')").arg(serve).arg(store).arg(brand);
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

SalesAndRefundsListPage::~SalesAndRefundsListPage()
{
    delete ui;
}
