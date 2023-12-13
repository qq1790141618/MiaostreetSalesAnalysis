#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>

namespace Ui {
class controller;
}

class controller : public QWidget
{
    Q_OBJECT

public:
    explicit controller(QWidget *parent = nullptr);
    ~controller();
    void setmaxIcon(bool maxis);
    void onlyShowClose();
    void setTitle(QString text, QChar icon);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void min();
    void max();
    void close();

private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_max_clicked();
    void on_pushButton_min_clicked();

private:
    Ui::controller *ui;
};

#endif // CONTROLLER_H
