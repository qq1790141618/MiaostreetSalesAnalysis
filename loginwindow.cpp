#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"

#include <QFontDatabase>
#include <QWidget>
#include <QTimer>
#include <QStandardPaths>
#include <QRegularExpressionValidator>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#include "./components/controller.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();

    iconfont = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/font/iconfont.ttf")).at(0);

    ui->lineEdit_account->setMaxLength(16);
    ui->lineEdit_password->setMaxLength(16);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    ui->lineEdit_target->setMaxLength(32);
    ui->lineEdit_code->setMaxLength(6);
    QRegularExpression regExp("[0-9]+");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, ui->lineEdit_code);
    ui->lineEdit_code->setValidator(validator);

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->primaryScreen()->availableGeometry()
        )
    );

    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

    ui->label_error_text->setWordWrap(true);
    ui->label_error_text2->setWordWrap(true);
    checklogin();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}

void LoginWindow::on_pushButton_change_clicked()
{
    if(loginMode == 0){
        loginMode = 1;
        ui->pushButton_change->setText(QStringLiteral("切换账号密码登录"));
    } else {
        loginMode = 0;
        ui->pushButton_change->setText(QStringLiteral("切换手机号登录"));
    }
    ui->stackedWidget->setCurrentIndex(loginMode);
}

void LoginWindow::on_pushButton_clicked()
{
    QString target = ui->lineEdit_target->text();

    if(target == ""){
        ui->label_error_text2->setText(QStringLiteral("发送目标不能为空"));
    } else {
        ui->lineEdit_target->setDisabled(true);
        ui->pushButton->setText(QStringLiteral("发送中.."));
        ui->pushButton->setDisabled(true);

        QTimer::singleShot(120, this, [=]{
            QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
            QNetworkRequest request(QUrl("https://work-serve.fixeam.com/api/user/send-code?target=" + target));
            QNetworkReply *reply = networkManager->get(request);

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray response = reply->readAll();
                    QString jsonString = QString::fromUtf8(response);
                    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

                    bool result = jsonDocument["result"].toBool();

                    if(result){
                        verifyId = jsonDocument["verify_id"].toString();
                        targetSended = ui->lineEdit_target->text();

                        timoput = 60;
                        QTimer *timer = new QTimer(this);
                        connect(timer, &QTimer::timeout, this, [=]{
                            timoput--;
                            ui->pushButton->setText(QString::number(timoput) + QStringLiteral("秒后重发"));

                            if(timoput == 0){
                                timer->stop();
                                ui->pushButton->setDisabled(false);
                            }
                        });
                        timer->start(1000);
                    } else {
                        QString error = jsonDocument["error"].toString();

                        ui->label_error_text2->setText(error);
                        ui->pushButton->setText(QStringLiteral("发送验证码"));
                        ui->pushButton->setDisabled(false);
                    }

                    ui->lineEdit_target->setDisabled(false);
                } else {
                    qDebug() << "Error: " << reply->errorString();

                    ui->label_error_text2->setText(QStringLiteral("验证码发送失败"));
                    ui->pushButton->setText(QStringLiteral("发送验证码"));
                    ui->pushButton->setDisabled(false);
                    ui->lineEdit_target->setDisabled(false);
                }

                reply->deleteLater();
            });
        });
    }
}

void LoginWindow::on_pushButton_login_clicked()
{
    ui->pushButton_login->setText(QStringLiteral("登录中..."));
    ui->pushButton_login->setDisabled(true);

    QTimer::singleShot(120, this, [=]{
        if(ui->stackedWidget->currentIndex() == 2){
            qiangzhi = true;
            checklogin();
        } else if(loginMode == 0){
            passwordLogin();
        } else if(loginMode == 1){
            codeLogin();
        }
    });
}

void LoginWindow::passwordLogin(){
    QString account = ui->lineEdit_account->text();
    QString password = ui->lineEdit_password->text();

    if(account == "" || password == ""){
        ui->label_error_text->setText(QStringLiteral("账号或密码不能为空"));

        if(account == ""){
            ui->lineEdit_account->setFocus();
        } else {
            ui->lineEdit_password->setFocus();
        }

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else {
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
        QNetworkRequest request(QUrl("https://work-serve.fixeam.com/api/user/login/bypassword?account=" + account + "&password=" + password));
        QNetworkReply *reply = networkManager->get(request);

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QString jsonString = QString::fromUtf8(response);
                QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

                bool result = jsonDocument["result"].toBool();
                if(!result){
                    QString error = jsonDocument["error"].toString();
                    ui->label_error_text->setText(error);

                    ui->pushButton_login->setDisabled(false);
                    ui->pushButton_login->setText(QStringLiteral("登录"));
                }

                QString token = jsonDocument["token"].toString();
                settings->beginGroup("USERLOGIN");
                settings->setValue("ACCESS_TOKEN", token);
                settings->endGroup();

                doNotChangeS = true;
                checklogin();
            } else {
                qDebug() << "Error: " << reply->errorString();

                ui->pushButton_login->setDisabled(false);
                ui->pushButton_login->setText(QStringLiteral("登录"));
            }

            reply->deleteLater();
        });
    }
}

void LoginWindow::codeLogin(){
    QString target = ui->lineEdit_target->text();
    QString code = ui->lineEdit_code->text();

    int length = code.length();
    if(targetSended.isNull()){
        ui->label_error_text2->setText(QStringLiteral("请先发送验证码"));

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else if(target == ""){
        ui->label_error_text2->setText(QStringLiteral("手机号/邮箱不能为空"));

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else if(target != targetSended){
        ui->label_error_text2->setText(QStringLiteral("手机号/邮箱与发送验证码的不一致"));

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else if(code == ""){
        ui->label_error_text2->setText(QStringLiteral("验证码不能为空"));

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else if(length != 6){
        ui->label_error_text2->setText(QStringLiteral("验证码长度不正确"));

        ui->pushButton_login->setDisabled(false);
        ui->pushButton_login->setText(QStringLiteral("登录"));
    } else {
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
        QNetworkRequest request(QUrl("https://work-serve.fixeam.com/api/user/login/bycode?target=" + target + "&verify-id=" + verifyId + "&code=" + code));
        QNetworkReply *reply = networkManager->get(request);

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QString jsonString = QString::fromUtf8(response);
                QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

                bool result = jsonDocument["result"].toBool();
                if(!result){
                    QString error = jsonDocument["error"].toString();
                    ui->label_error_text2->setText(error);

                    ui->pushButton_login->setDisabled(false);
                    ui->pushButton_login->setText(QStringLiteral("登录"));
                }

                QString token = jsonDocument["token"].toString();
                settings->beginGroup("USERLOGIN");
                settings->setValue("ACCESS_TOKEN", token);
                settings->endGroup();

                doNotChangeS = true;
                checklogin();
            } else {
                qDebug() << "Error: " << reply->errorString();

                ui->pushButton_login->setDisabled(false);
                ui->pushButton_login->setText(QStringLiteral("登录"));
            }

            reply->deleteLater();
        });
    }
}

void LoginWindow::on_lineEdit_account_textChanged(const QString &arg1)
{
    ui->label_error_text->setText("");
}

void LoginWindow::on_lineEdit_password_textChanged(const QString &arg1)
{
    ui->label_error_text->setText("");
}

void LoginWindow::on_lineEdit_target_textChanged(const QString &arg1)
{
    ui->label_error_text2->setText("");
}

void LoginWindow::on_lineEdit_code_textChanged(const QString &arg1)
{
    ui->label_error_text2->setText("");
}

void LoginWindow::on_pushButton_logother_clicked()
{
    ui->stackedWidget->setCurrentIndex(loginMode);
    ui->pushButton_change->show();
}

void LoginWindow::checklogin(){
    settings->beginGroup("USERLOGIN");
    token = settings->value("ACCESS_TOKEN", "").toString();
    settings->endGroup();

    settings->beginGroup("SETTINGS");
    bool doNotAutoLog = settings->value("DISABLE_AUTO_LOG", false).toBool();
    bool logOutTemp = settings->value("DISABLE_LOG_OUT_TEMP", false).toBool();
    if(logOutTemp){
        settings->remove("DISABLE_LOG_OUT_TEMP");
    }
    settings->endGroup();

    if(token != ""){
        if(!doNotChangeS){
            ui->stackedWidget->setCurrentIndex(2);
            ui->pushButton_change->hide();

            int size = 120;
            int round = size * 0.5;

            settings->beginGroup("USERLOGIN");
            QString headImage = settings->value("HEAD_IMAGE", "").toString();
            QPixmap pixmap(headImage);
            pixmap = pixmap.scaled(QSize(size, size), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPixmap roundedPixmap(pixmap.size());
            roundedPixmap.fill(Qt::transparent);
            QPainter painter(&roundedPixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            QPainterPath path;
            path.addRoundedRect(roundedPixmap.rect(), round, round);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, pixmap);

            ui->label_head->setPixmap(roundedPixmap);

            QString nickname = settings->value("USER_NICKNAME", "").toString();
            ui->label_nickname->setText(nickname);
            settings->endGroup();

            if(!qiangzhi){
                if(doNotAutoLog || logOutTemp){
                    return;
                }
            }

            ui->pushButton_login->setText(QStringLiteral("登录中..."));
            ui->pushButton_login->setDisabled(true);
        }

        ui->pushButton_logother->setDisabled(true);

        QTimer::singleShot(120, this, [=]{
            QNetworkAccessManager *networkManager = new QNetworkAccessManager;
            QNetworkRequest request(QUrl("https://work-serve.fixeam.com/api/user/login/verify?token=" + token));
            QNetworkReply *reply = networkManager->get(request);

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray response = reply->readAll();
                    QString jsonString = QString::fromUtf8(response);
                    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

                    bool result = jsonDocument["result"].toBool();
                    if(!result){
                        QString error = jsonDocument["error"].toString();
                        ui->label_error_text->setText(error);

                        ui->stackedWidget->setCurrentIndex(loginMode);
                        ui->pushButton_login->setDisabled(false);
                        ui->pushButton_login->setText(QStringLiteral("登录"));
                        ui->pushButton_change->show();
                        ui->pushButton_logother->setDisabled(false);
                    }

                    MainWindow *w = new MainWindow();
                    w->show();
                    hide();

                    QString headsrc = jsonDocument["user"]["headsrc"].toString();
                    QString headimage = saveHeadImage(headsrc);
                    QString nickname = jsonDocument["user"]["nickname"].toString();
                    settings->beginGroup("USERLOGIN");
                    settings->setValue("USER_NICKNAME", nickname);
                    settings->setValue("HEAD_IMAGE", headimage);
                    settings->endGroup();

                    close();
                } else {
                    qDebug() << "Error: " << reply->errorString();

                    ui->stackedWidget->setCurrentIndex(loginMode);
                    ui->pushButton_login->setDisabled(false);
                    ui->pushButton_login->setText(QStringLiteral("登录"));
                    ui->pushButton_change->show();
                    ui->pushButton_logother->setDisabled(false);
                }

                reply->deleteLater();
            });
        });
    }
}

QString LoginWindow::saveHeadImage(const QString &imageUrl){
    settings->beginGroup("SETTINGS");
    QString cache = settings->value("CACHE_STORAGE","").toString();
    if(cache == ""){
        cache = "D:/MiaostreetSalesAnalysis/cache/";
        settings->setValue("CACHE_STORAGE", cache);
    }
    settings->endGroup();

    QString tempFilePath = cache + "head-image/" + calculateMD5(imageUrl) + ".jpg";

    // 检查文件夹是否存在，如果不存在则创建它
    QString folderPath = tempFilePath.section('/', 0, -2); //-2表示从倒数第二个'/'开始的字符串
    QDir dir(folderPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QFile tempFile(tempFilePath);

    if (tempFile.exists()) {
        return tempFilePath;
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

                    return tempFilePath;
                }
            }
        });
    }
}

void LoginWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
    {
        on_pushButton_login_clicked();
    }
        break;
    default:
        break;
    }
}
