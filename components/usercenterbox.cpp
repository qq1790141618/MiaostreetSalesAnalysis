#include "usercenterbox.h"
#include "ui_usercenterbox.h"

UserCenterBox::UserCenterBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserCenterBox)
{
    ui->setupUi(this);
}

UserCenterBox::~UserCenterBox()
{
    delete ui;
}
