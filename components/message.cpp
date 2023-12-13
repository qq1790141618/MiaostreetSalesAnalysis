#include "message.h"
#include "ui_message.h"

#include <QGraphicsDropShadowEffect>

Message::Message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
}

void Message::setIcon(QPixmap pixmap){
    pixmap = pixmap.scaled(QSize(20, 20), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_message_icon->setPixmap(pixmap);
}

void Message::setIconFont(QChar chars){
    ui->label_message_icon->setText(chars);
}

void Message::setText(QString text){
    ui->label_message_content->setText(text);
    this->setToolTip(text);
}

Message::~Message()
{
    delete ui;
}
