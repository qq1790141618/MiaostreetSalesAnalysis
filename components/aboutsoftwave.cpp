#include "aboutsoftwave.h"
#include "ui_aboutsoftwave.h"

#include "controller.h"
#include <QGraphicsDropShadowEffect>

AboutSoftwave::AboutSoftwave(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutSoftwave)
{
    ui->setupUi(this);
    ui->menubar->hide();
    ui->statusbar->hide();
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setWindowTitle(QStringLiteral("关于"));

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#a6a6a6"));
    ui->centralwidget->setGraphicsEffect(shadow);

    QPixmap pixmapLogo(":/images/logo.png");
    pixmapLogo = pixmapLogo.scaled(QSize(100, 100), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_logo->setPixmap(pixmapLogo);

    QPixmap pixmapMiaostreetLogo(":/images/miaostreet.png");
    pixmapMiaostreetLogo = pixmapMiaostreetLogo.scaled(QSize(18, 18), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_miaostreet->setPixmap(pixmapMiaostreetLogo);

    controller *close_control = new controller(this);
    close_control->onlyShowClose();
    close_control->setTitle(QStringLiteral("关于"), QChar(0xe6e5));
    connect(close_control, &controller::close, this, [=]{
        this->close();
    });
    ui->verticalLayout->addWidget(close_control);

    this->setFixedSize(300, 420);
}

AboutSoftwave::~AboutSoftwave()
{
    delete ui;
}

void AboutSoftwave::setVersionName(QString name){
    ui->label_version->setText(QStringLiteral("版本：") + name);
};

void AboutSoftwave::mousePressEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}

void AboutSoftwave::mouseMoveEvent(QMouseEvent *e){
    if(e->buttons()&Qt::LeftButton)
        move(e->pos() + pos() - clickPos);
}

void AboutSoftwave::on_pushButton_clicked()
{
    this->close();
    emit checkUpdate();
}

