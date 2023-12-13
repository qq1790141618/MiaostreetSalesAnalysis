#include "controller.h"
#include "ui_controller.h"

controller::controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controller)
{
    ui->setupUi(this);
    ui->pushButton_close->installEventFilter(this);
    ui->pushButton_min->setText(QChar(0xe65d));
    ui->pushButton_max->setText(QChar(0xe664));
    ui->pushButton_close->setText(QChar(0xe646));
}

controller::~controller()
{
    delete ui;
}

void controller::on_pushButton_close_clicked()
{
    emit close();
}

void controller::on_pushButton_max_clicked()
{
    emit max();
}

void controller::on_pushButton_min_clicked()
{
    emit min();
}

void controller::setmaxIcon(bool maxis){
    if(maxis){
        ui->pushButton_max->setText(QChar(0xe664));
    } else {
        ui->pushButton_max->setText(QChar(0xe692));
    }
}

bool controller::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui->pushButton_close && event->type() == QEvent::Enter)
    {
        ui->pushButton_close->setStyleSheet("background-color: #d54941; color: #ffffff;");
    }
    else if (obj == ui->pushButton_close && event->type() == QEvent::Leave)
    {
        ui->pushButton_close->setStyleSheet("");
    }

    return QWidget::eventFilter(obj, event);
}

void controller::onlyShowClose(){
    ui->pushButton_min->hide();
    ui->pushButton_max->hide();
}

void controller::setTitle(QString text, QChar icon){
    ui->label_con_title_title->setText(text);
    ui->label_con_title_icon->setText(icon);
}

