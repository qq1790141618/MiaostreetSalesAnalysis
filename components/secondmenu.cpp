#include "secondmenu.h"
#include "ui_secondmenu.h"

#include <QFile>
#include <QJsonDocument>
#include <QPushButton>

SecondMenu::SecondMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondMenu)
{
    ui->setupUi(this);
    QFile file(":/json-data/second-menus.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray jsonData = file.readAll();
            menuArray = QJsonDocument::fromJson(
                        QString::fromUtf8(jsonData)
                        .toUtf8()
                        ).array();
    }
}

SecondMenu::~SecondMenu()
{
    delete ui;
}

void SecondMenu::on_menu_search_textChanged(const QString &arg1)
{
    if(arg1 == ""){
        mackMenu(first_menu, "menu");
    } else {
        mackMenu(arg1, "search");
    }
}

void SecondMenu::mackMenu(QString keyword, QString mode){
    show();

    if (!ui->verticalLayout_second_menus->isEmpty()) {
        while (QLayoutItem* item = ui->verticalLayout_second_menus->takeAt(0)) {
            QWidget* dwidget = item->widget();
            if (dwidget) {
                delete dwidget;
            }
            delete item;
        }
    }

    if(keyword == "about" || keyword == "set"){
        this->hide();
    } else {
        this->show();
    }
    if(mode == "menu"){
        first_menu=keyword;
    }

    for(auto it = menuArray.begin(); it != menuArray.end(); ++it) {
        QJsonValue value = *it;

        if((mode == "menu" && value["parent"] == keyword) ||
                            (mode == "search" &&
                             (keyword == "/"
                              || value["page"].toString().contains(keyword)
                              || value["title"].toString().contains(keyword)
                              || value["description"].toString().contains(keyword))))
        {
            QPushButton *button = new QPushButton(this);

            button->setText(" " + value["title"].toString() + " ");
            button->setToolTip(value["description"].toString());
            button->setCursor(Qt::PointingHandCursor);
            if(value["page"] == second_menu){
                button->setStyleSheet("color: #366ef4; border-color: #618dff; background-color: #d9e1ff;");
            }
            button->setIcon(QIcon(QPixmap(value["icon"].toString())));
            button->setIconSize(QSize(24, 24));
            button->setProperty("targetPage", value["page"]);

            connect(button, &QPushButton::clicked, this, [=]{
                QString page = value["page"].toString();
                QString parent = value["parent"].toString();

                if(second_menu != page || nearClick == page){
                    second_menu = page;
                    active_first_menu = parent;
                    setActiveMenu();
                } else {
                    nearClick = page;
                    QTimer::singleShot(300, this, [=]{
                        nearClick = "";
                    });
                }
            });

            ui->verticalLayout_second_menus->addWidget(button);
        }
    }
}

void SecondMenu::setActiveMenu()
{
    emit changeSecondMenu(second_menu, active_first_menu);

    QList<QPushButton*> buttonList = ui->widget_menu_list->findChildren<QPushButton*>();
    foreach(QPushButton* button, buttonList) {
        if(button->property("targetPage").toString() == second_menu){
            button->setStyleSheet("color: #366ef4; border-color: #618dff; background-color: #d9e1ff;");
        } else {
            button->setStyleSheet("");
        }
    }
}

