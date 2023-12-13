#ifndef ABOUTSOFTWAVE_H
#define ABOUTSOFTWAVE_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class AboutSoftwave;
}

class AboutSoftwave : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutSoftwave(QWidget *parent = nullptr);
    ~AboutSoftwave();
    void setVersionName(QString name);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void checkUpdate();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AboutSoftwave *ui;
    QPoint clickPos;
};

#endif // ABOUTSOFTWAVE_H
