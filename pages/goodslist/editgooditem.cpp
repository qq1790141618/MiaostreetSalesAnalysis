#include "editgooditem.h"
#include "ui_editgooditem.h"
#include "bridge.h"

EditGoodItem::EditGoodItem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditGoodItem)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    ui->statusbar->hide();
    ui->stackedWidget->setCurrentIndex(1);

    QMovie* movie = new QMovie(":/images/loading-2.gif");
    movie->setScaledSize(QSize(20, 20));
    ui->label_gif->setMovie(movie);
    ui->label_gif->movie()->start();

    webview = new QWebEngineView();
    webview->setContextMenuPolicy(Qt::NoContextMenu);
    webview->page()->setBackgroundColor(QColor("#f2f3ff"));
    ui->verticalLayout->addWidget(webview);

    QWebChannel *webChannel = new QWebChannel();
    Bridge *webobj = new Bridge();
    webChannel->registerObject("webobj", webobj);
    webview->page()->setWebChannel(webChannel);
    connect(webobj, &Bridge::closeIt, this, [=]{
        close();
    });
    connect(webobj, &Bridge::showIt, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
    });
}

void EditGoodItem::setStylenumber(QString serve, QString store, QString brand, QString name){
    stylenumber = name;
    this->setWindowTitle(QStringLiteral("商品编辑-") + name);

    webview->load(QUrl("qrc:/pages/goodslist/edit.html"));
    connect(webview, &QWebEngineView::loadFinished, this, [=] {
        QString javascriptCode = QString("createPage('%1', '%2', '%3', '%4')").arg(serve).arg(store).arg(brand).arg(stylenumber);
        webview->page()->runJavaScript(javascriptCode);
    });
}

EditGoodItem::~EditGoodItem()
{
    delete ui;
}
