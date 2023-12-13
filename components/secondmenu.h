#ifndef SECONDMENU_H
#define SECONDMENU_H

#include <QWidget>
#include <QJsonArray>
#include <QTimer>

namespace Ui {
class SecondMenu;
}

class SecondMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SecondMenu(QWidget *parent = nullptr);
    ~SecondMenu();
    void mackMenu(QString keyword, QString mode);
    void setSecondMenu(QString menu){
        second_menu = menu;
    };

protected:
    void setActiveMenu();

signals:
    void changeSecondMenu(const QString &secondMenu, const QString &firstMenu);

private slots:
    void on_menu_search_textChanged(const QString &arg1);

private:
    Ui::SecondMenu *ui;
    QJsonArray menuArray;
    QString first_menu = "home";
    QString active_first_menu = "home";
    QString second_menu = "home";
    QString nearClick = "";
};

#endif // SECONDMENU_H
