#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCryptographicHash>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void passwordLogin();
    void codeLogin();
    void checklogin();
    QString saveHeadImage(const QString &imageUrl);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_pushButton_change_clicked();
    void on_pushButton_clicked();
    void on_pushButton_login_clicked();
    void on_lineEdit_account_textChanged(const QString &arg1);
    void on_lineEdit_password_textChanged(const QString &arg1);
    void on_lineEdit_target_textChanged(const QString &arg1);
    void on_lineEdit_code_textChanged(const QString &arg1);
    void on_pushButton_logother_clicked();

private:
    Ui::LoginWindow *ui;
    QPoint clickPos;
    QFont iconfont;
    QString token;
    bool doNotChangeS = false;
    bool qiangzhi = false;

    int loginMode = 0;
    QString targetSended = nullptr;
    QString verifyId = nullptr;
    int timoput;
    QSettings *settings;

    QString calculateMD5(const QString &input)
    {
        QByteArray data = input.toUtf8();

        // 计算 MD5 值
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(data);
        QByteArray md5 = hash.result();

        // 将 MD5 值转换为字符串
        QString md5Str = md5.toHex();

        return md5Str;
    }
};

#endif // LOGINWINDOW_H
