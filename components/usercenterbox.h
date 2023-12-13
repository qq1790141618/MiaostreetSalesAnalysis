#ifndef USERCENTERBOX_H
#define USERCENTERBOX_H

#include <QMainWindow>

namespace Ui {
class UserCenterBox;
}

class UserCenterBox : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserCenterBox(QWidget *parent = nullptr);
    ~UserCenterBox();

private:
    Ui::UserCenterBox *ui;
};

#endif // USERCENTERBOX_H
