#ifndef FIRSTMENU_H
#define FIRSTMENU_H

#include <QWidget>
#include <QSettings>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMouseEvent>

namespace Ui {
class MenuFirstLevel;
}

class MenuFirstLevel : public QWidget
{
    Q_OBJECT

public:
    explicit MenuFirstLevel(QWidget *parent = nullptr);
    ~MenuFirstLevel();
    void setActiveMenus();
    void setActiveFirstMenus(const QString &firstmenu){
        activeFirstMenu = firstmenu;
    };
    void setMax(const bool &max){
        isMax = max;
    };
    void checkSecondMenuSet();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void initMenus();
    void createLogoAction();
    void createUsrAction();
    void changeSecondMenuShow();
    void openContextMenu(QPushButton *pushButton);

signals:
    void changeFirstMenu(QString name);
    void mackSecondMenu(QString name);
    void aboutShow();
    void settingsShow();
    void max();
    void min();
    void exit();
    void logout();
    void hideSecondMenu(bool status);
    void addPage(const QString &name, const QString &parent);

private:
    Ui::MenuFirstLevel *ui;
    QSettings *settings;
    bool user_click = true;
    bool isMax = false;
    QString activeMenu = "pushButton_menu_home";
    QString activeFirstMenu = "home";
    bool isSecondMenuHide = false;
    QJsonArray menuArray;
};

#endif // FIRSTMENU_H
