#include "closeconfirm.h"
#include "ui_closeconfirm.h"

#include <QGraphicsDropShadowEffect>
#include <QStandardPaths>
#include <QSettings>

#include "controller.h"

CloseConfirm::CloseConfirm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CloseConfirm)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowModality(Qt::ApplicationModal);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#a6a6a6"));
    ui->centralwidget->setGraphicsEffect(shadow);

    controller *close_control = new controller(this);
    close_control->onlyShowClose();
    close_control->setTitle(QStringLiteral("关闭软件"), QChar(0xe663));
    connect(close_control, &controller::close, this, [=]{
        this->close();
    });
    ui->verticalLayout_2->addWidget(close_control);
}

CloseConfirm::~CloseConfirm()
{
    delete ui;
}

void CloseConfirm::on_pushButton_confirm_clicked()
{
    QString behavior;
    if(ui->radioButton_hide->isChecked()){
        behavior = "hide";
    }
    if(ui->radioButton_close->isChecked()){
        behavior = "close";
    }
    if(ui->checkBox_donottip->isChecked()){
        QString configFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QSettings *settings = new QSettings(configFilePath + "/config.ini", QSettings::IniFormat);
        settings->beginGroup("SETTINGS");
        settings->setValue("MAINWINDOW_CLOSE_BUTTON", behavior);
        settings->setValue("MAINWINDOW_CLOSE_BUTTON_MESSAGE", 1);
        settings->endGroup();
    }
    emit confirm(behavior);
    this->close();
}

void CloseConfirm::on_pushButton_cancel_clicked()
{
    this->close();
}

void CloseConfirm::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void CloseConfirm::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}

