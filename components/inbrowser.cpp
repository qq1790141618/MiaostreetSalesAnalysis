#include "inbrowser.h"
#include "ui_inbrowser.h"

#include <QStandardPaths>
#include <QTimer>
#include <QStyle>
#include <QScreen>
#include <QFontDatabase>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QGraphicsDropShadowEffect>
#include <QMovie>
#include <QFileDialog>

inbrowser::inbrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::inbrowser)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();
    this->setAttribute(Qt::WA_QuitOnClose, false);

    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
    qint64 windowWidth = settings->value("BROWSER_WIDTH", 0).toInt();
    qint64 windowHeight = settings->value("BROWSER_HEIGHT", 0).toInt();
    if(windowWidth != 0 && windowHeight != 0){
        this->resize(windowWidth, windowHeight);
    } else {
        this->resize(420, 880);
    }
    QFont iconfont = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/font/iconfont.ttf")).at(0);

    QPixmap pixmap(":/images/inbrowser.png");
    this->setWindowIcon(pixmap);
    this->setWindowTitle(QStringLiteral("Web浏览组件"));
    pixmap = pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_icon->setPixmap(pixmap);

    ui->pushButton_prev->setText(QChar(0xe679));
    ui->pushButton_prev->setEnabled(false);
    ui->pushButton_next->setText(QChar(0xe6a3));
    ui->pushButton_next->setEnabled(false);
    ui->pushButton_more->setText(QChar(0xe684));

    webview = new QWebEngineView(this);
    webview->setStyleSheet("QMenu { border-radius: 7px; background-color: #ffffff; margin: 1px; }\
                           QMenu::item{ padding: 6px 24px; font-size: 9pt; }\
                           QMenu::item:selected { background-color: #f3f3f3; }");
    ui->verticalLayout_content->addWidget(webview);
    connect(webview, &QWebEngineView::loadStarted, this, [=]{
        QMovie* movie = new QMovie(":/images/loading-2.gif");
        movie->setScaledSize(QSize(20, 20));
        ui->label_icon->setMovie(movie);
        ui->label_icon->movie()->start();
    });
    connect(webview, &QWebEngineView::loadFinished, this, &inbrowser::onPageLoadFinished);
    connect(webview->page(), &QWebEnginePage::urlChanged, this, &inbrowser::onPageUrlChanged);
    connect(webview, &QWebEngineView::iconUrlChanged, this, &inbrowser::handleIconUrlChanged);
    connect(webview, &QWebEngineView::windowTitleChanged, this, &inbrowser::onPageUrlChanged);

    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

inbrowser::~inbrowser()
{
    delete ui;
}

void inbrowser::setUrl(QString url){
    mainUrl = QUrl(url);
    mainIndex = 0;
    maxIndex = mainIndex;

    QFontMetrics fontWidth(ui->label_title->font());
    QString elideNote = fontWidth.elidedText(url, Qt::ElideRight, ui->label_title->width());
    ui->label_title->setText(elideNote);
    this->setToolTip(url);

    webview->load(mainUrl);
}

void inbrowser::onPageLoadFinished(bool ok)
{
    if (ok) {
        ui->label_icon->movie()->stop();
        ui->label_icon->setMovie(new QMovie());

        QString title = webview->title();
        QFontMetrics fontWidth(ui->label_title->font());
        QString elideNote = fontWidth.elidedText(title, Qt::ElideRight, ui->label_title->width());
        ui->label_title->setText(elideNote);
        this->setToolTip(title);
    }
}

void inbrowser::onPageUrlChanged(const QUrl& url){
    if(mainUrl == url){
        return;
    }
    if(!record){
        record = true;
        return;
    }
    mainUrl = url;

    mainIndex++;
    maxIndex = mainIndex;

    ui->pushButton_prev->setEnabled(true);
}

void inbrowser::handleIconUrlChanged(const QUrl &url)
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QNetworkReply* reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();

            QImage image = QImage::fromData(imageData);
            QPixmap pixmap = QPixmap::fromImage(image);

            pixmap = pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_icon->setPixmap(pixmap);
        }

        reply->deleteLater();
    });
}

void inbrowser::resizeEvent(QResizeEvent *event){
    QSize size = event->size();

    settings->beginGroup("SETTINGS");
    settings->setValue("BROWSER_WIDTH", size.width());
    settings->setValue("BROWSER_HEIGHT", size.height());
    settings->endGroup();

    QWidget::resizeEvent(event);
}

void inbrowser::on_pushButton_more_clicked()
{
    QMenu *moreOptions = new QMenu(this);
    moreOptions->setAttribute(Qt::WA_TranslucentBackground);
    moreOptions->setWindowFlags(moreOptions->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    moreOptions->setCursor(Qt::PointingHandCursor);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#a6a6a6"));
    shadow->setBlurRadius(6);
    moreOptions->setGraphicsEffect(shadow);

    moreOptions->setStyleSheet("QMenu { border-radius: 7px; background-color: #ffffff; padding: 7px 1px; margin: 5px; }\
                                QMenu::item{ padding: 6px 24px; font-size: 9pt; }\
                                QMenu::item:selected { background-color: #f3f3f3; }");

    QAction *action_copyUrl = moreOptions->addAction(QStringLiteral("复制链接"));
    connect(action_copyUrl, &QAction::triggered, this, [=]{
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(mainUrl.toString());
    });
    QAction *action_openInBrowser = moreOptions->addAction(QStringLiteral("使用默认浏览器打开"));
    connect(action_openInBrowser, &QAction::triggered, this, [=]{
        QDesktopServices::openUrl(mainUrl.toString());
    });
    QAction *action_refresh = moreOptions->addAction(QStringLiteral("刷新 (Ctrl+R | F5)"));
    connect(action_refresh, &QAction::triggered, this, [=]{
        webview->reload();
    });

    moreOptions->exec(ui->pushButton_more->mapToGlobal(QPoint(-5, ui->pushButton_more->height() + 3)));
}

void inbrowser::on_pushButton_prev_clicked()
{
    mainIndex--;
    record = false;
    webview->back();
    mainUrl = webview->url();

    ui->pushButton_next->setEnabled(true);
    if(mainIndex == 0){
        ui->pushButton_prev->setEnabled(false);
    }
}

void inbrowser::on_pushButton_next_clicked()
{
    mainIndex++;
    record = false;
    webview->forward();
    mainUrl = webview->url();

    ui->pushButton_prev->setEnabled(true);
    if(mainIndex == maxIndex){
        ui->pushButton_next->setEnabled(false);
    }
}

void inbrowser::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_R) {
        webview->reload();
    } else if (event->key() == Qt::Key_F5) {
        webview->reload();
    } else {
        QWidget::keyPressEvent(event);
    }
}
