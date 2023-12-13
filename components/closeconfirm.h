#ifndef CLOSECONFIRM_H
#define CLOSECONFIRM_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class CloseConfirm;
}

class CloseConfirm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloseConfirm(QWidget *parent = nullptr);
    ~CloseConfirm();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void confirm(QString behavior);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_confirm_clicked();

private:
    Ui::CloseConfirm *ui;
    QPoint clickPos;
};

#endif // CLOSECONFIRM_H
