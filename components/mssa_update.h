#ifndef MSSA_UPDATE_H
#define MSSA_UPDATE_H

#include <QMainWindow>
#include "framelesswindow.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMouseEvent>

namespace Ui {
class mssa_update;
}

class mssa_update : public CFramelessWindow
{
    Q_OBJECT

public:
    explicit mssa_update(QWidget *parent = nullptr);
    ~mssa_update();
    void checkUpdate(QString serve, int version_id, QString version_name, QString version_type);
    void setUserClick(bool is){
        user_click = is;
    }

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void updatelaunch();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void updateDataReadProgress(qint64,qint64);
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_install_clicked();
    void on_pushButton_local_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::mssa_update *ui;
    bool user_click = false;
    QPoint clickPos;
    QString package;
    QNetworkReply *reply;
    QString filePath;
    int newVersionId;
};

#endif // MSSA_UPDATE_H
