#ifndef EDITGOODITEM_H
#define EDITGOODITEM_H

#include <QMainWindow>
#include <QtWebEngineWidgets>

namespace Ui {
class EditGoodItem;
}

class EditGoodItem : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditGoodItem(QWidget *parent = nullptr);
    ~EditGoodItem();
    void setStylenumber(QString serve, QString store, QString brand, QString name);
    QString getStylenumber(){
        return stylenumber;
    }

private:
    Ui::EditGoodItem *ui;
    QString stylenumber;
    QWebEngineView *webview;
};

#endif // EDITGOODITEM_H
