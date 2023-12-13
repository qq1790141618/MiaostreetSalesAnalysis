#include "mssa_update.h"
#include "ui_mssa_update.h"

#include <QDebug>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QProcess>
#include <QMovie>

#include "controller.h"

mssa_update::mssa_update(QWidget *parent) :
    CFramelessWindow(parent),
    ui(new Ui::mssa_update)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();
    this->setAttribute(Qt::WA_QuitOnClose, false);

    controller *updater_c = new controller(this);
    updater_c->onlyShowClose();
    updater_c->setTitle(QStringLiteral("软件更新"), QChar(0xe738));
    connect(updater_c, &controller::close, this, [=]{
        this->close();
    });
    ui->verticalLayout_updater_c->addWidget(updater_c);

    QMovie* movie = new QMovie(":/images/loading-2.gif");
    movie->setScaledSize(QSize(20, 20));
    ui->label_loading->setMovie(movie);
    ui->label_loading->movie()->start();
}

mssa_update::~mssa_update()
{
    delete ui;
}

void mssa_update::checkUpdate(QString serve, int version_id, QString version_name, QString version_type){
    QPixmap pixmap = QPixmap(":/update/3f000613-a487-4d1d-8221-7fb15051ad39.png");
    pixmap = pixmap.scaled(QSize(500, 171), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_image->setPixmap(pixmap);
    ui->stackedWidget->setCurrentIndex(3);
    this->setFixedSize(QSize(500, 180));

    ui->label_old->setText(version_name);
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(serve + "/app/version/latest?platform=windows&version_type=" + version_type));
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QString jsonString = QString::fromUtf8(response);
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

            int id = jsonDocument["version_id"].toInt();
            QString type = jsonDocument["version_type"].toString();
            QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
            QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
            settings->beginGroup("SETTINGS");
            int donottip = settings->value("DO_NOT_TIP", 0).toInt();
            settings->endGroup();
            if(id == donottip && !user_click){
                return;
            }

            QString vs = jsonDocument["version"].toString();

            if(id > version_id){
                newVersionId = id;
                package = jsonDocument["package_resource"].toString();
                int size = jsonDocument["package_size"].toInt();
                QString sizeShow = QString::number(size / 1024 / 1024) + "MB";

                ui->label_new->setText(vs + type);
                ui->label_size->setText(sizeShow);
                ui->label_tip_1->setText(QStringLiteral("更新内容：") + jsonDocument["package_update_information"].toString());
                this->show();

                QPixmap pixmap = QPixmap(":/update/9e9cf7e2-96ed-40f0-ae5c-c79bed086c56.png");
                pixmap = pixmap.scaled(QSize(500, 171), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->label_image->setPixmap(pixmap);
                ui->stackedWidget->setCurrentIndex(0);
                this->setFixedSize(QSize(500, 350));
            } else {
                QPixmap pixmap = QPixmap(":/update/ad303c0e-27c2-4323-935b-4327fffc1ff8.png");
                pixmap = pixmap.scaled(QSize(500, 171), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->label_image->setPixmap(pixmap);
                ui->stackedWidget->setCurrentIndex(4);
                this->setFixedSize(QSize(500, 180));
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    });
}

void mssa_update::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void mssa_update::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        if (clickPos.y() <= 50) {
            move(e->pos() + pos() - clickPos);
        }
}

void mssa_update::on_pushButton_2_clicked()
{
    this->close();
}

void mssa_update::on_pushButton_3_clicked()
{
    this->close();
    reply->deleteLater();
}

void mssa_update::on_pushButton_5_clicked()
{
    this->close();
}

void mssa_update::on_pushButton_clicked()
{
    QPixmap pixmap = QPixmap(":/update/166c9d65-188f-4b39-8c21-9fc0a1357766.png");
    pixmap = pixmap.scaled(QSize(500, 145), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_image->setPixmap(pixmap);
    this->setFixedSize(QSize(500, 260));
    ui->stackedWidget->setCurrentIndex(1);

    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
    settings->beginGroup("SETTINGS");
    QString cache = settings->value("CACHE_STORAGE", "D:/MiaostreetSalesAnalysis/cache/").toString();
    settings->endGroup();

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request(package);
    QUrl packageUrl = QUrl(package);
    QString filename = packageUrl.fileName();
    filePath = cache + "package/" + filename;
    // 检查文件夹是否存在，如果不存在则创建它
    QString folderPath = filePath.section('/', 0, -2); //-2表示从倒数第二个'/'开始的字符串
    QDir dir(folderPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    request.setAttribute(QNetworkRequest::User, filePath);

    reply = manager->get(request);
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDataReadProgress(qint64,qint64)));
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadWrite)) {
                file.write(reply->readAll());
                file.close();

                QPixmap pixmap = QPixmap(":/update/dcf98ca0-8ee9-4886-b702-88e9ec61188c.png");
                pixmap = pixmap.scaled(QSize(500, 114), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->label_image->setPixmap(pixmap);
                ui->stackedWidget->setCurrentIndex(2);
                this->setFixedSize(QSize(500, 220));
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    });
}

void mssa_update::updateDataReadProgress(qint64 percent, qint64 size){
    int pr = percent * 100 / size;
    ui->progressBar->setValue(pr);
};

void mssa_update::on_pushButton_install_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    emit updatelaunch();
}

void mssa_update::on_pushButton_local_clicked()
{
    QFileInfo fileInfo(filePath);
    auto pathDir = fileInfo.path();
    QString strFilePath = "file:///" + pathDir;
    QDesktopServices::openUrl(QUrl(strFilePath));
}

void mssa_update::on_pushButton_4_clicked()
{
    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
    settings->beginGroup("SETTINGS");
    settings->setValue("DO_NOT_TIP", newVersionId);
    settings->endGroup();
    close();
}

