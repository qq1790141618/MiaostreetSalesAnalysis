#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "framelesswindow.h"

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>

#include <QSettings>
#include <QMouseEvent>

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void initWindowStyle();
    void addControl();
    quint64 dirFileSize(const QString &path);
    void initCombox();
    void readSet();
    void computerCache();
    bool DeleteFileOrFolder(const QString &strPath);

private slots:
    void on_pushButton_shop_clicked();
    void on_pushButton_base_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_chooseFold_clicked();
    void on_pushButton_storage_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_account_clicked();
    void on_pushButton_logout_clicked();

    void on_checkBox_autolog_stateChanged(int arg1);

    void on_comboBox_close_button_currentIndexChanged(int index);

signals:
    void changeSetting(QString store, QString brand);
    void cacheCleared();
    void logout();

private:
    Ui::SettingWindow *ui;
    QPoint clickPos;

    QSettings *settings;
    QString store = "132";
    QString brand = QStringLiteral("兔皇");
    QString cache;
    QString closeButton;
};

#endif // SETTINGWINDOW_H
