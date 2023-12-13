#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QSettings>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QProcess>
#include <QSettings>

#include "./components/inbrowser.h"
#include "./pages/goodslist/editgooditem.h"
#include "./pages/analysis/view.h"

class Bridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content MEMBER m_content NOTIFY contentChanged) //该属性可由页面访问

public:
    Bridge(){};
    QString getContent(){return m_content;}

signals:
    void contentChanged(QString nc);
    void closeIt();
    void showIt();
    void open(QString page);

public slots:
    void test(const QString &text) //该函数是页面端调用的
    {
        qDebug()<<text;
    }
    void show(){
        emit showIt();
    }
    void close(){
        emit closeIt();
    }
    void openImage(const QString &imageUrl){
        QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

        settings->beginGroup("SETTINGS");
        QString cache = settings->value("CACHE_STORAGE","").toString();
        if(cache == ""){
            cache = "D:/MiaostreetSalesAnalysis/cache/";
            settings->setValue("CACHE_STORAGE", cache);
        }
        settings->endGroup();

        QString tempFilePath = cache + "temp-image/" + calculateMD5(imageUrl) + ".jpg";
        // 检查文件夹是否存在，如果不存在则创建它
        QString folderPath = tempFilePath.section('/', 0, -2); //-2表示从倒数第二个'/'开始的字符串
        QDir dir(folderPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QFile tempFile(tempFilePath);

        if (tempFile.exists()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
        } else {
            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
            QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(imageUrl)));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray imageData = reply->readAll();

                    QFile tempFile(tempFilePath);
                    if (tempFile.open(QIODevice::WriteOnly)) {
                        tempFile.write(imageData);
                        tempFile.close();

                        QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
                    }
                }
            });
        }
    }
    void cachingImage(const QString &imageUrl){
        QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

        settings->beginGroup("SETTINGS");
        QString cache = settings->value("CACHE_STORAGE","").toString();
        if(cache == ""){
            cache = "D:/MiaostreetSalesAnalysis/cache/";
            settings->setValue("CACHE_STORAGE", cache);
        }
        settings->endGroup();

        QString tempFilePath = cache + "temp-image/" + calculateMD5(imageUrl) + ".jpg";
        // 检查文件夹是否存在，如果不存在则创建它
        QString folderPath = tempFilePath.section('/', 0, -2); //-2表示从倒数第二个'/'开始的字符串
        QDir dir(folderPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        QFile tempFile(tempFilePath);

        if (!tempFile.exists())  {
            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
            QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(imageUrl)));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray imageData = reply->readAll();

                    QFile tempFile(tempFilePath);
                    if (tempFile.open(QIODevice::WriteOnly)) {
                        tempFile.write(imageData);
                        tempFile.close();
                    }
                }
            });
        }
    }
    void openImageInBrowser(const QString &imageUrl){
        inbrowser *webview = new inbrowser();
        webview->setUrl(imageUrl);
        webview->show();
    }
    void openMobileUrl(const QString &url){
        inbrowser *webview = new inbrowser();
        webview->setUrl(url);
        webview->show();
    }
    void openUrl(const QString &url){
        QDesktopServices::openUrl(QUrl::fromLocalFile(url));
    }
    void copyImage(QString imageUrl){
        QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
        QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(imageUrl)));

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray imageData = reply->readAll();

                QImage image = QImage::fromData(imageData);
                QClipboard* clipboard = QApplication::clipboard();
                clipboard->setImage(image, QClipboard::Clipboard);
            }

            reply->deleteLater();
        });
    }
    void copyUrl(QString url){
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(url);
    }
    void saveImage(QString imageUrl){
        QString saveFilePath = QFileDialog::getSaveFileName(new QWidget(), "保存图片", "", "Image Files (*.jpg *.png)");

        if (!saveFilePath.isEmpty()) {
            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
            QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(imageUrl)));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QFile file(saveFilePath);

                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(reply->readAll());
                        file.close();
                    }
                }

                reply->deleteLater();
            });
        }
    }
    void saveXlsx(QString url){
        QString saveFilePath = QFileDialog::getSaveFileName(new QWidget(), QStringLiteral("保存Excel模板"), QStringLiteral("模板"), "Excel Files (*.xlsx)");

        if (!saveFilePath.isEmpty()) {
            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
            QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(url)));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QFile file(saveFilePath);

                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(reply->readAll());
                        file.close();
                    }
                }

                reply->deleteLater();
            });
        }
    }
    void saveZip(QString url){
        QString saveFilePath = QFileDialog::getSaveFileName(new QWidget(), QStringLiteral("保存压缩包模板"), QStringLiteral("模板"), "Window Zip Files (*.zip)");

        if (!saveFilePath.isEmpty()) {
            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
            QNetworkReply* reply = networkManager->get(QNetworkRequest(QUrl(url)));

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QFile file(saveFilePath);

                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(reply->readAll());
                        file.close();
                    }
                }

                reply->deleteLater();
            });
        }
    }
    void storage(QString group, QString key, QString value){
        QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
        settings->beginGroup(group);
        settings->setValue(key, value);
        settings->endGroup();
    }
    void editGoodItem(QString serve, QString store, QString brand, QString stylenumber){
        EditGoodItem *deitModal = new EditGoodItem();
        deitModal->setStylenumber(serve, store, brand, stylenumber);
        deitModal->show();
    }
    void viewSalesTask(const QString &serve, const QString &id){
        view *v = new view();
        v->setId(id);
        v->show();
    }
    void openPage(const QString &page){
        emit open(page);
    }

private:
    QString m_content;
    QString calculateMD5(const QString &input)
    {
        QByteArray data = input.toUtf8();

        // 计算 MD5 值
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(data);
        QByteArray md5 = hash.result();

        // 将 MD5 值转换为字符串
        QString md5Str = md5.toHex();

        return md5Str;
    }
};

#endif // BRIDGE_H
