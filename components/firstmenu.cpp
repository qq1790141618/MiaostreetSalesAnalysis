#include "firstmenu.h"
#include "ui_firstmenu.h"

#include <QPropertyAnimation>
#include <QListView>
#include <QPushButton>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QPainterPath>

MenuFirstLevel::MenuFirstLevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuFirstLevel)
{
    ui->setupUi(this);

    QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

    QFile file(":/json-data/second-menus.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = file.readAll();
        menuArray = QJsonDocument::fromJson(
                        QString::fromUtf8(jsonData)
                            .toUtf8()
                        ).array();
    }

    initMenus();
    setActiveMenus();
}

MenuFirstLevel::~MenuFirstLevel()
{
    delete ui;
}

void MenuFirstLevel::checkSecondMenuSet(){
    settings->beginGroup("SETTINGS");
    if(settings->value("HIDE_SECOND_MENU", 0).toInt() == 1){
        changeSecondMenuShow();
    }
    settings->endGroup();
}

bool MenuFirstLevel::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(obj == ui->label_logo){
            createLogoAction();
        }
        if(obj == ui->label_user){
            createUsrAction();
        }
        return true;
    }
    return false;
}

void MenuFirstLevel::initMenus(){
    QPixmap pixmapLogo(":/images/logo.png");
    pixmapLogo = pixmapLogo.scaled(QSize(36, 36), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_logo->setPixmap(pixmapLogo);
    ui->label_logo->setCursor(Qt::PointingHandCursor);
    ui->label_logo->installEventFilter(this);
//    ui->label_logo->hide();

    int size = 36;
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

    ui->label_user->setPixmap(roundedPixmap);
    ui->label_user->installEventFilter(this);
    ui->label_user->setCursor(Qt::PointingHandCursor);

    QString nickname = settings->value("USER_NICKNAME", "").toString();
    ui->label_user->setToolTip(nickname);

    settings->endGroup();

    ui->pushButton_menu_home->setProperty("unused", 0xe6b8);
    ui->pushButton_menu_home->setProperty("actived", 0xe6bb);
    ui->pushButton_menu_home->setProperty("value", "home");
    connect(ui->pushButton_menu_home, &QPushButton::clicked, this, [=]{
        activeMenu="home";
        setActiveMenus();
    });
    ui->pushButton_menu_goods->setProperty("unused", 0xe6cd);
    ui->pushButton_menu_goods->setProperty("actived", 0xe778);
    ui->pushButton_menu_goods->setProperty("value", "goods");
    connect(ui->pushButton_menu_goods, &QPushButton::clicked, this, [=]{
        activeMenu="goods";
        setActiveMenus();
    });
    ui->pushButton_menu_order->setProperty("unused", 0xe77d);
    ui->pushButton_menu_order->setProperty("actived", 0xe77c);
    ui->pushButton_menu_order->setProperty("value", "orders");
    connect(ui->pushButton_menu_order, &QPushButton::clicked, this, [=]{
        activeMenu="orders";
        setActiveMenus();
    });
    ui->pushButton_menu_import->setProperty("unused", 0xe75d);
    ui->pushButton_menu_import->setProperty("actived", 0xe75d);
    ui->pushButton_menu_import->setProperty("value", "import");
    connect(ui->pushButton_menu_import, &QPushButton::clicked, this, [=]{
        activeMenu="import";
        setActiveMenus();
    });
    ui->pushButton_menu_data->setProperty("unused", 0xe722);
    ui->pushButton_menu_data->setProperty("actived", 0xe721);
    ui->pushButton_menu_data->setProperty("value", "analysis");
    connect(ui->pushButton_menu_data, &QPushButton::clicked, this, [=]{
        activeMenu="analysis";
        setActiveMenus();
    });
    ui->pushButton_menu_info->setProperty("unused", 0xe6e5);
    ui->pushButton_menu_info->setProperty("value", "about");
    connect(ui->pushButton_menu_info, &QPushButton::clicked, this, [=]{
        emit aboutShow();
    });
    ui->pushButton_menu_setting->setProperty("unused", 0xe605);
    ui->pushButton_menu_setting->setProperty("value", "set");
    connect(ui->pushButton_menu_setting, &QPushButton::clicked, this, [=]{
        emit settingsShow();
    });
    ui->pushButton_hide_second_menu->setProperty("unused", 0xe609);
    ui->pushButton_hide_second_menu->setProperty("value", "hide");
    connect(ui->pushButton_hide_second_menu, &QPushButton::clicked, this, &MenuFirstLevel::changeSecondMenuShow);
}

void MenuFirstLevel::setActiveMenus(){
    QList<QPushButton*> buttonList = this->findChildren<QPushButton*>();

    foreach(QPushButton* button, buttonList) {
        QString btname = button->objectName();
        QChar unused = button->property("unused").toChar();
        QChar actived = button->property("actived").toChar();
        QString page = button->property("value").toString();
        QString name = button->text();

        if(page == activeFirstMenu){
            button->setText(actived);
            button->setStyleSheet("QPushButton{ background-color: #f2f3ff; color: #8eabff; }");
            emit changeFirstMenu(page);
            if(activeFirstMenu == activeMenu && user_click){
                if(isSecondMenuHide){
                    openContextMenu(button);
                } else {
                    emit mackSecondMenu(page);
                }
            }
        } else if(page == activeMenu) {
            button->setText(unused);
            button->setStyleSheet("QPushButton{ background-color: #e8e8e8; }");
            emit changeFirstMenu(page);
            if(isSecondMenuHide && user_click){
                openContextMenu(button);
            } else {
                emit mackSecondMenu(page);
            }
        } else {
            button->setText(unused);
            button->setStyleSheet("");
        }
    }

    user_click = true;
}

void MenuFirstLevel::createLogoAction(){
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
                                QMenu::item{ padding: 6px 36px; font-size: 9pt; }\
                                QMenu::item:selected { background-color: #f3f3f3; }");

    QAction *action_official_site = moreOptions->addAction(QStringLiteral("打开Web端"));
    connect(action_official_site, &QAction::triggered, this, [=]{
        settings->beginGroup("USERLOGIN");
        QString token = settings->value("ACCESS_TOKEN", "").toString();
        settings->endGroup();

        QDesktopServices::openUrl(QUrl("https://work.fixeam.com/?access_token=" + token));
    });

    QAction *action_min = moreOptions->addAction(QStringLiteral("最小化"));
    connect(action_min, &QAction::triggered, this, [=]{
        emit min();
    });

    QAction *action_max = moreOptions->addAction(QStringLiteral("最大化"));
    connect(action_max, &QAction::triggered, this, [=]{
        emit max();
    });
    if(isMax){
        action_max->setDisabled(true);
    }

    QAction *action_exit = moreOptions->addAction(QStringLiteral("退出软件"));
    connect(action_exit, &QAction::triggered, this, [=]{
        emit exit();
    });

    moreOptions->exec(ui->label_logo->mapToGlobal(QPoint(ui->label_logo->width() + 2, 12)));
}

void MenuFirstLevel::createUsrAction(){
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
                                QMenu::item{ padding: 6px 36px; font-size: 9pt; }\
                                QMenu::item:selected { background-color: #f3f3f3; }");

    QAction *action_viewHead = moreOptions->addAction(QStringLiteral("查看头像"));
    connect(action_viewHead, &QAction::triggered, this, [=]{
        settings->beginGroup("USERLOGIN");
        QString headImage = settings->value("HEAD_IMAGE", "").toString();
        QDesktopServices::openUrl(QUrl::fromLocalFile(headImage));
        settings->endGroup();
    });

    QAction *action_logout = moreOptions->addAction(QStringLiteral("退出登录"));
    connect(action_logout, &QAction::triggered, this, [=]{
        settings->beginGroup("SETTINGS");
        settings->setValue("DISABLE_LOG_OUT_TEMP", true);
        settings->endGroup();
        emit logout();
    });

    moreOptions->exec(ui->label_user->mapToGlobal(QPoint(ui->label_user->width() + 2, 0)));
}

void MenuFirstLevel::changeSecondMenuShow(){
    isSecondMenuHide = !isSecondMenuHide;
    settings->beginGroup("SETTINGS");
    if(isSecondMenuHide){
        settings->setValue("HIDE_SECOND_MENU", 1);

//        ui->widget_first_menu->setStyleSheet("#widget_first_menu{ border: 1px solid #f3f3f3; }");
        ui->pushButton_hide_second_menu->setProperty("unused", 0xe608);
        ui->pushButton_hide_second_menu->setToolTip(QStringLiteral("显示二级菜单"));
        emit hideSecondMenu(true);
    } else {
        settings->remove("HIDE_SECOND_MENU");

//        ui->widget_first_menu->setStyleSheet("");
        ui->pushButton_hide_second_menu->setProperty("unused", 0xe609);
        ui->pushButton_hide_second_menu->setToolTip(QStringLiteral("隐藏二级菜单"));
        emit hideSecondMenu(false);
    }
    settings->endGroup();
    QChar unused = ui->pushButton_hide_second_menu->property("unused").toChar();
    ui->pushButton_hide_second_menu->setText(unused);
}

void MenuFirstLevel::openContextMenu(QPushButton *pushButton){
    QString page = pushButton->property("value").toString();

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
                                QMenu::item{ padding: 6px 36px; font-size: 9pt; }\
                                QMenu::item:selected { background-color: #f3f3f3; }");

    for(auto it = menuArray.begin(); it != menuArray.end(); ++it) {
        QJsonValue value = *it;

        if(value["parent"] == page)
        {
            QAction *action = moreOptions->addAction(value["title"].toString());
            connect(action, &QAction::triggered, this, [=]{
                QString page = value["page"].toString();
                QString parent = value["parent"].toString();
                activeFirstMenu = parent;

                user_click = false;
                emit addPage(page, parent);
            });
        }
    }
    connect(moreOptions, &QMenu::aboutToHide, this, [=]{
        activeMenu = "";
        setActiveMenus();
    });

    moreOptions->exec(pushButton->mapToGlobal(QPoint(pushButton->width() + 2, 0)));
}
