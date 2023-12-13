#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

namespace Ui {
class Message;
}

class Message : public QWidget
{
    Q_OBJECT

public:
    explicit Message(QWidget *parent = nullptr);
    ~Message();
    void setIcon(QPixmap pixmap);
    void setIconFont(QChar chars);
    void setText(QString text);

private:
    Ui::Message *ui;
};

#endif // MESSAGE_H
