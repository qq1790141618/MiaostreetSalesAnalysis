#include "orderlist.h"
#include "ui_orderlist.h"

#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QStandardPaths>

OrderList::OrderList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderList)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [=]{
        QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);

        settings->beginGroup("USERLOGIN");
        QString token = settings->value("ACCESS_TOKEN", "").toString();
        settings->endGroup();

        QDesktopServices::openUrl(QUrl("https://work.fixeam.com/orders/list?access_token=" + token + "&store=" + store + "&brand=" + brand));
    });
    connect(ui->pushButton_order, &QPushButton::clicked, this, [=]{
        emit openPage("import");
    });
}

OrderList::~OrderList()
{
    delete ui;
}
